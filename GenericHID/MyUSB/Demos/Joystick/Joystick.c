/*
MyUSB Library
Copyright (C) Dean Camera, 2007.

dean [at] fourwalledcubicle [dot] com
www.fourwalledcubicle.com

Released under the GPL Licence, Version 3
*/

/*
Joystick demonstration application. This gives a simple reference
application for implementing a USB Keyboard device, for USB Joysticks
using the standard Keyboard HID profile.

This device will show up as a generic joystick device, with two buttons.
Pressing the joystick inwards is the first button, and the HWB button
on the USBKEY board is the second.

Moving the joystick on the USBKEY moves the joystick location on the
host computer.
*/

#include <avr/eeprom.h>
#include <avr/wdt.h>
#include "Common.h"
#include "Serial.h"
#include "Joystick.h"
#include "Controls.h"
#include "PWM.h"

#include <string.h>


static void ProcessOutputReport( byte *buf );

//! Declare function pointer to USB bootloader entry point
void (*start_bootloader) (void)=(void (*)(void))0xf000;
#define RUN_BOOTLOADER	((byte *)(E2END))

/* Scheduler Task ID list */
TASK_ID_LIST
{
    USB_USBTask_ID,
    USB_Joystick_Report_ID,
};

/* Scheduler Task List */
TASK_LIST
{
    { TaskID: USB_USBTask_ID,		TaskName: USB_USBTask,		TaskStatus: TASK_RUN  },
    { TaskID: USB_Joystick_Report_ID,	TaskName: USB_Joystick_Report,	TaskStatus: TASK_RUN  },
};

byte DynamicHIDData[MAX_HID_DATA] __attribute__ ((section (".noinit")));			// buffer to copy eeprom to.
static struct SApplicationHeader *pApplicationHdr;	// Pointer to the application data header
static byte *pApplicationData;				// pointer to the application data - description and working data for installed components.
static byte nUseStatusLEDs;				// Bit mask indicating whether to use USBKeyLeds 1, 2 or none
byte bSerialDebug;					// RS232 debug enabled flag
byte bUSBDebug;						// Debug via USB line enabled flag
static bool bInitialised;
static byte bPreScaler;					// remember the clock prescaler so we can restore it when we 
							// jump to the boot loader.
uint16_t g_nCounterTop = 0xFFFF;
uint16_t g_nCounterCount;
volatile uint16_t g_nCounterCounter;
uint16_t g_nCounterMask;

volatile uint16_t milliseconds;				// use to count 1000ms to clock 1 sec.
volatile uint8_t seconds_counter;			// second counter - only used for debug at the moment.
volatile uint8_t milliseconds_counter;			// msec counter - wraps at 0xFF - used for key debounce
volatile uint16_t milliseconds_counter16;		// msec counter - wraps at 0xFFFF - used for timing last message.


#define FORCE_PACKET_TIME		1000		// how often to force send packet
#define MINIMUM_PACKET_TIME		  50		// 50ms - 20 updates/sec
static uint16_t nLastPacketTime;			// last time we sent a packet
static bool bForcePacket;				// this is global (in this module) because USB may be busy
// when we want to send a new packet (but unlikely if its busy we probably don't need to send)

static byte ReadBufferPtr;					// pointer to where we last received a byte - in case packet is split (can this happen?)
static byte ReadBuffer[JOYSTICK_OUT_EPSIZE+1];// Read buffer. +1 for report ID
static byte *pReportLengths;				// pointer to table of report lengths
static byte BytesToRead;					// When reading a report, contains the number of bytes still to read.
enum ReadStates								// Report reading state machine states
{
    ReadReportId,
    ReadData
};
static enum ReadStates ReadState;			// Report reading state machine state
static byte LastSendBuffer[JOYSTICK_IN_EPSIZE];		// Last in report sent.  Remember this so we can do a diff and not send the same
// report over and over again.

// millisecond timer.  Count milliseconds and seconds.
SIGNAL (TIMER2_COMPA_vect) { Bicolour_Init( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED1, nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED2); while ( 1) { Bicolour_SetLed(1, BICOLOUR_LED1_GREEN); _delay_ms(100); Bicolour_SetLed(1, BICOLOUR_LED1_RED); _delay_ms(100); } }
SIGNAL (TIMER2_COMPB_vect) { Bicolour_Init( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED1, nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED2); while ( 1) { Bicolour_SetLed(1, BICOLOUR_LED1_GREEN); _delay_ms(100); Bicolour_SetLed(1, BICOLOUR_LED1_RED); _delay_ms(100); } }
SIGNAL (TIMER2_OVF_vect)  { Bicolour_Init( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED1, nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED2); while ( 1) { Bicolour_SetLed(1, BICOLOUR_LED1_GREEN); _delay_ms(100); Bicolour_SetLed(1, BICOLOUR_LED1_RED); _delay_ms(100); } }


SIGNAL (TIMER0_COMPA_vect)
{
    milliseconds_counter16++;
    milliseconds_counter++;
    milliseconds++;
    if ( milliseconds == 1000 )
    {
	seconds_counter++;
	milliseconds = 0;
    }

    g_nCounterCount++;
    if ( g_nCounterCount >= g_nCounterTop )
    {
	g_nCounterCounter++;
	g_nCounterCount = 0;
    }
}

// General IO initialisation.  We only do the timer here.  Other ports, those with components connected are initialised 
// after USB has connected.
static void ioint(void)
{
    milliseconds = 0;
    seconds_counter = 0;
    milliseconds_counter = 0;
    milliseconds_counter16 = 0;

    // initialise timer0 as the ms timer.  Used to 1sec and msec times.
    TCCR0A = _BV(WGM01);		    // Mode 2
    TCCR0B = _BV(CS01) | _BV(CS00);	    // clk/64
    OCR0A = 125;
    TIMSK0 |= _BV(OCIE0A);		    // interupt on output compare A

    PRR0 = _BV(PRTWI) | _BV(PRSPI);	    // We never use TWI or SPI
    ASSR = 0;
}

int main(void)
{
    byte nReason = MCUSR;

    // Watch dog is used by the bootloader to reset after firmware upgrades
    wdt_reset(); 
    MCUSR &= ~(1 << WDRF); 
    wdt_disable();

    MCUSR = 0;

    if ( (nReason & _BV(WDRF)) && eeprom_read_byte( RUN_BOOTLOADER ) == 0 )
    {
	eeprom_write_byte( RUN_BOOTLOADER, 0xFF );
	_delay_ms( 500/8 );
	(*start_bootloader)();           //! Jumping to bootloader
    }
    eeprom_write_byte( RUN_BOOTLOADER, 0xFF );

    /* Disable Clock Division */
    bPreScaler = CLKPR;
    cli();
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    /* Read the dynamic HID configuration from eeprom */
    int nBlockLen = eeprom_read_word(0);
    eeprom_read_block( DynamicHIDData, 0, nBlockLen);

    pApplicationHdr = (struct SApplicationHeader *)(DynamicHIDData + (((struct SDynamicHID *)DynamicHIDData)->nApplicationDataOffset));
    //byte bBusPowered = pApplicationHdr->nOptions & DEVICE_OPTION_BUS_POWERED;
    nUseStatusLEDs = pApplicationHdr->nOptions & DEVICE_OPTION_USE_USBKEY_LEDS;
    // not used yet byte bHIDDebug = pApplicationHdr->nOptions & DEVICE_OPTION_HID_DEBUG;
    bSerialDebug = pApplicationHdr->nOptions & DEVICE_OPTION_SERIAL_DEBUG;
    byte b5Volt = pApplicationHdr->nOptions & DEVICE_OPTION_5V;
    pReportLengths = pApplicationHdr->ReportLength;
    pApplicationData = (byte *)pApplicationHdr + sizeof(struct SApplicationHeader);
    ReadBufferPtr = 0;
    ReadState = ReadReportId;
    bInitialised = false;
    nLastPacketTime = 0;
    bForcePacket = false;

    /* Hardware Initialization */
    ioint();

    initialisePWMTimers( pApplicationHdr->timers, countof(pApplicationHdr->timers) );

    if ( nUseStatusLEDs )
    {
	Bicolour_Init( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED1, nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED2);
	if ( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED1 )
	    Bicolour_SetLed(1, BICOLOUR_LED1_ORANGE);
	if ( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED2 )
	    Bicolour_SetLed(2, BICOLOUR_LED2_ORANGE);	
    }

    if ( bSerialDebug )
	UART1_Init( 12 );		// Fixed at 38400,8,n,1

    sei();

    if ( bSerialDebug )
    {
	UART1_Send_P( PSTR("USB HID v1.0 Initialised (") );
	UART1_SendHex( nReason );
	UART1_Send_P( PSTR(")\r\n") );
    }

    /* Initial LED colour - Double red to indicate USB not ready */
    if ( nUseStatusLEDs )
    {
	if ( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED1 )
	    Bicolour_SetLed(1, BICOLOUR_LED1_RED);
	if ( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED2 )
	    Bicolour_SetLed(2, BICOLOUR_LED2_RED);																				    
    }

    /* Initialize USB Subsystem */
    if ( b5Volt )
	USB_Init(USB_MODE_DEVICE, USB_DEV_OPT_HIGHSPEED | USB_OPT_REG_ENABLED);
    else
	USB_Init(USB_MODE_DEVICE, USB_DEV_OPT_HIGHSPEED);

    /* Scheduling - routine never returns, so put this last in the main function */
    Scheduler_Start();
}


enum Class_Request
{
    CLASSREQ_GET_REPORT     = 1,
    CLASSREQ_GET_IDLE       = 2,
    CLASSREQ_GET_PROTOCOL   = 3,
    CLASSREQ_SET_REPORT     = 9,
    CLASSREQ_SET_IDLE       = 10,
    CLASSREQ_SET_PROTOCOL   = 11
};

enum ReportType
{
    ReportType_Input = 1,
    ReportType_Output = 2,
    ReportType_Feature = 3
};

EVENT_HANDLER(USB_UnhandledControlPacket)
{
    //const uint8_t Request, 
    //const uint8_t RequestType

    /* Process specific control requests */
    if ( bSerialDebug )
    {
	UART1_Send_P(PSTR("UCP="));
	UART1_SendHex( Request );
	UART1_Send_P(PSTR(","));
	UART1_SendHex( RequestType );
	UART1_SendCRLF();
    }

    if ( ((RequestTypeBitfield_t *)&RequestType)->Type == REQTYPE_Class &&
	Request == CLASSREQ_GET_REPORT )
    {
	byte nReportId = Endpoint_Read_Byte();
	byte type = Endpoint_Read_Byte();
	int nEndPoint = Endpoint_Read_Word();	// Index
	int nRequestLen = Endpoint_Read_Word();	// len

	Endpoint_ClearSetupReceived();


	if ( bSerialDebug )
	{
	    UART1_Send_P(PSTR("GET_REPORT="));
	    UART1_SendHex( type );
	    UART1_Send_P(PSTR(","));
	    UART1_SendHex( nReportId );
	    UART1_Send_P(PSTR(","));
	    UART1_SendHex( nEndPoint );
	    UART1_Send_P(PSTR(","));
	    UART1_SendHex( nRequestLen );
	    UART1_SendCRLF();
	}

	if ( type == ReportType_Feature )
	{
	    WriteFeatureReport( nReportId, pApplicationData );

	    Endpoint_In_Clear();

	    while (!(Endpoint_Out_IsReceived()));
	    Endpoint_Out_Clear();
	}
	else if ( type == ReportType_Input && nReportId == 1 )	    // we only have 1 input report
	{
	    byte nLen = 0;
	    byte buf[JOYSTICK_IN_EPSIZE];

	    ReadControls( nReportId, pApplicationData, buf, &nLen );

	    //Endpoint_SelectEndpoint(nEndPoint);
	    Endpoint_Write_Byte(nReportId);			// Report ID - input report is always 1
	    if ( bSerialDebug )
	    {
		UART1_SendHex( nReportId );
		UART1_SendChar(' ');
	    }

	    for ( byte i = 0; i < nLen; i++ )
	    {
		Endpoint_Write_Byte(buf[i]);
		if ( bSerialDebug )
		{
		    UART1_SendHex( buf[i] );
		    UART1_SendChar(' ');
		}
	    }
	    if ( bSerialDebug )
	    {
		UART1_SendCRLF();
	    }

	    Endpoint_In_Clear();

	    while (!(Endpoint_Out_IsReceived()))
		continue;
	    Endpoint_Out_Clear();
	}
	else
	{
	    Endpoint_In_Clear();
	}
    }
    else if ( ((RequestTypeBitfield_t *)&RequestType)->Type == REQTYPE_Class && Request == CLASSREQ_SET_IDLE )
    {
	// ignore idle requests.
	byte value = Endpoint_Read_Word();
	byte index = Endpoint_Read_Word();
	//byte length = Endpoint_Read_Word();

	if ( bSerialDebug )
	{
	    UART1_Send_P(PSTR("SET_IDLE="));
	    UART1_SendHex( value );
	    UART1_Send_P(PSTR(","));
	    UART1_SendHex( index );
	    UART1_SendCRLF();
	}

	Endpoint_ClearSetupReceived();
	Endpoint_In_Clear();
    }
    else if ( Request == REQ_ClearFeature )
    {
	uint16_t wValue = Endpoint_Read_Word_LE();
	uint16_t wIndex = Endpoint_Read_Word_LE();

	if ( bSerialDebug )
	{
	    UART1_Send_P(PSTR("CLEAR_FEATURE="));
	    UART1_SendHex( wValue );
	    UART1_Send_P(PSTR(","));
	    UART1_SendHex( wIndex );
	    UART1_SendCRLF();
	}
	Endpoint_ClearSetupReceived();
	Endpoint_In_Clear();
    }
}

EVENT_HANDLER(USB_CreateEndpoints)
{
    // Allocate the largest endpoint buffers.  We only have two, so even at 64 bytes, double buffered, we only use 256 of 836 bytes
    Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPNUM, EP_TYPE_INTERRUPT,
	ENDPOINT_DIR_IN, JOYSTICK_IN_EPSIZE,
	ENDPOINT_BANK_DOUBLE);

    Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPNUM, EP_TYPE_INTERRUPT,
	ENDPOINT_DIR_OUT, JOYSTICK_OUT_EPSIZE,
	ENDPOINT_BANK_DOUBLE);

    /* Double green to indicate USB connected and ready */
    if ( nUseStatusLEDs )
    {
	if ( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED1 )
	    Bicolour_SetLed(1, BICOLOUR_LED1_GREEN);
	if ( nUseStatusLEDs & DEVICE_OPTION_USE_USBKEY_LED2 )
	    Bicolour_SetLed(2, BICOLOUR_LED2_GREEN);		
    }

    if ( pApplicationHdr->nPowerPort != 0xFF )
    {
	SetDDR( pApplicationHdr->nPowerPort, DDR_OUTPUT );
	SetIOBit( pApplicationHdr->nPowerPort );
    }
}

TASK(USB_Joystick_Report)
{
    if ( !bInitialised )
    {
	if ( bSerialDebug )
	    UART1_Send_P( PSTR("Initialising\r\n") );

	InitControls( pApplicationData );
	bInitialised = true;

	if ( bSerialDebug )
	    UART1_Send_P( PSTR("Initialisation Complete\r\n") );
    }

    {
	static uint16_t t = 0;
	static uint8_t old = 0;
	t++;
	if ( old != seconds_counter )
	{
	    old = seconds_counter;
	    if ( bSerialDebug )
	    {
		UART1_SendInt( t );
		UART1_SendCRLF();
	    }
	    t = 0;
	}
    }

    // Also send a packet if we haven't for n msec.
    if ( milliseconds_counter16 - nLastPacketTime > FORCE_PACKET_TIME )
	bForcePacket = true;

    // Poll the controls, make a report if the data has changed.
    // todo - set limit of report every n msec
    byte nLen = 0;
    byte buf[JOYSTICK_IN_EPSIZE];
    ReadControls( 1, pApplicationData, buf, &nLen );

    if ( ( (milliseconds_counter16 - nLastPacketTime) > MINIMUM_PACKET_TIME  && memcmp( buf, LastSendBuffer, nLen ) != 0 ) 
	|| bForcePacket )
    {
	if (USB_IsConnected)
	{
	    Endpoint_SelectEndpoint(JOYSTICK_IN_EPNUM);

	    if (Endpoint_ReadWriteAllowed() && Endpoint_In_IsReady() )
	    {
		Endpoint_Write_Byte(1);			// Report ID - input report is always 1
		if ( bSerialDebug )
		{
		    UART1_SendHex( 1 );
		    UART1_SendChar(' ');
		}

		for ( byte i = 0; i < nLen; i++ )
		{
		    Endpoint_Write_Byte(buf[i]);
		    if ( bSerialDebug )
		    {
			UART1_SendHex( buf[i] );
			UART1_SendChar(' ');
		    }
		}
		if ( bSerialDebug )
		{
		    UART1_SendCRLF();
		}
		Endpoint_In_Clear();

		memcpy( LastSendBuffer, buf, nLen );
		nLastPacketTime = milliseconds_counter16;
		bForcePacket = false;
	    }
	}
    }

    if ( USB_IsConnected )
    {
	Endpoint_SelectEndpoint(JOYSTICK_OUT_EPNUM);
	if (Endpoint_Out_IsReceived() )
	{
	    if ( bSerialDebug )
	    {
		UART1_Send_P( PSTR("Received ") );
	    }

	    while ( Endpoint_BytesInEndpoint() )
	    {
		byte b = Endpoint_Read_Byte();
		if ( bSerialDebug )
		{
		    UART1_SendHex( b );
		    UART1_SendChar( ' ' );
		}

		switch ( ReadState )
		{
		    case ReadReportId:
			if ( b > 0 && b < MAX_REPORTS )
			{
			    ReadBufferPtr = 0;
			    ReadBuffer[ReadBufferPtr++] = b;
			    BytesToRead = pReportLengths[b-1];
			    if ( bSerialDebug )
			    {
				UART1_Send_P( PSTR("BytesToRead=") );
				UART1_SendHex( BytesToRead );
				UART1_SendChar( ' ' );
			    }
			    ReadState = ReadData;
			}
			else
			{
			    Endpoint_Out_Clear();
			}
			break;

		    case ReadData:
			ReadBuffer[ReadBufferPtr++] = b;
			BytesToRead--;
			if ( BytesToRead == 0 )
			{
			    ProcessOutputReport( ReadBuffer );
			    ReadState = ReadReportId;
			}
			break;
		}
	    }
	    Endpoint_Out_Clear();
	    if ( bSerialDebug )
	    {
		UART1_SendCRLF();
	    }
	}
    }
}

#define BOOTLOADER_REPORT	3

static void ProcessOutputReport( byte *buf )
{
    if ( bSerialDebug ) UART1_Send_P( PSTR("Processing Output Report\r\n") );

    if ( *buf == BOOTLOADER_REPORT )
    {				
	// DFU report.  Data is UInt32.  Check the value = 0xDF0DF0DF then restart.
	if ( *((uint32_t *)(buf+1)) == MAGIC_BOOTLOADER_CODE )
	{
	    if ( bSerialDebug )
	    {
		UART1_Send_P( PSTR("Starting Bootloader\r\n") );
		// Wait for the message to be sent.
		while ( !UART1_SendBufferEmpty() )
		    continue;
	    }

	    //// disable interrupts we may have started
	    //USB_ShutDown();
	    //TIMSK0 = 0;	// timers
	    //TIMSK1 = 0;
	    //TIMSK2 = 0;
	    //TIMSK3 = 0;
	    //TCCR0B = 0;
	    //TCCR1B = 0;
	    //TCCR2B = 0;
	    //TCCR3B = 0;
	    //UCSR1B = 0;	// usart
	    //ADCSRA = 0;	// dac
	    //EIMSK = 0;	// external interrupts
	    //PCICR = 0;
	    //PCMSK0 = 0;

	    //// Reset ports
	    //DDRA = 0; PORTA = 0;
	    //DDRB = 0; PORTB = 0;
	    //DDRC = 0; PORTC = 0;
	    //DDRD = 0; PORTD = 0;
	    //DDRE = 0; PORTE = 0;
	    //DDRF = 0; PORTF = 0;

	    // Use the watch dog to reset the MCU.  Set a flag in the eeprom to run bootloader
	    // when we start.
	    eeprom_write_byte( RUN_BOOTLOADER, 0 );
	    wdt_enable(WDTO_15MS);
	    for (;;) {}

	    //_delay_ms( 100 );		// 1/2sec delay

	    ////cli();
	    ////CLKPR = (1 << CLKPCE);
	    ////CLKPR = bPreScaler;
	    ////sei();

	    ////_delay_ms( 50 );

	    //(*start_bootloader)();           //! Jumping to booltoader
	}
    }
    else
    {
	WriteControls( *buf, pApplicationData, buf + 1 );	
    }
}








/* ToDO

optimise
compile with optimise.
Debug
serial/usb debug properly
*/

