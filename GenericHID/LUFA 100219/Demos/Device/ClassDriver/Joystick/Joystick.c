// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.



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

static void GenericHIDProcessing(void);
static void ProcessOutputReport( byte *buf );

//! Declare function pointer to USB bootloader entry point
#if defined(AT90USBKEY)
    void (*start_bootloader) (void)=(void (*)(void))0xf000;
#elif defined(TEENSYPP)
    void (*start_bootloader) (void)=(void (*)(void))0xfE00;
#else
    #error No Device Defined
#endif

#define RUN_BOOTLOADER	((byte *)(E2END))

byte DynamicHIDData[MAX_HID_DATA] __attribute__ ((section (".noinit")));			// buffer to copy eeprom to.
static struct SApplicationHeader *pApplicationHdr;	// Pointer to the application data header
static byte *pApplicationData;				// pointer to the application data - description and working data for installed components.
static byte nUseStatusLEDs;				// Bit mask indicating whether to use USBKeyLeds 1, 2 or none
byte nSerialDebugLevel;					// RS232 debug level
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


// Read from program memory.
static void LoadDynamicHIDData(void)
{
    // Specify the Dynamic HID Data memory address in program memory at END - MAX_BOOTLOADER - 4k
#define DYNAMIC_HIDDATA_ADDRESS	    (0x10000 - 0x1000 - 0x1000)	    // (this is actually 4k words)
    int nBlockLen = pgm_read_word(DYNAMIC_HIDDATA_ADDRESS);
    memcpy_P( DynamicHIDData, (PGM_P)DYNAMIC_HIDDATA_ADDRESS, nBlockLen );
}


// General IO initialisation.  We only do the timer here.  Other ports, those with components connected are initialised 
// after USB has connected.
static void ioint(void)
{
    milliseconds = 0;
    seconds_counter = 0;
    milliseconds_counter = 0;
    milliseconds_counter16 = 0;

#if F_CPU == 8000000
    // For AT90USBKey 

    // initialise timer0 as the ms timer.  Used for 1sec and msec times.
    TCCR0A = _BV(WGM01);		    // Mode 2
    TCCR0B = _BV(CS01) | _BV(CS00);	    // clk/64
    OCR0A = 125;

#elif F_CPU == 16000000
    // For Teensy++ 2.0

    // initialise timer0 as the ms timer.  Used for 1sec and msec times.
    TCCR0A = _BV(WGM01);		    // Mode 2
    TCCR0B = _BV(CS01) | _BV(CS00);	    // clk/64
    OCR0A = 250;
#else
    #error Unsupported clock speed
#endif

    TIMSK0 |= _BV(OCIE0A);		    // interupt on output compare A
    PRR0 = _BV(PRTWI);	    // We never use TWI
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

    /* Read the dynamic HID configuration */
    LoadDynamicHIDData();

    pApplicationHdr = (struct SApplicationHeader *)(DynamicHIDData + (((struct SDynamicHID *)DynamicHIDData)->nApplicationDataOffset));

    if ( !(pApplicationHdr->nOptions & DEVICE_OPTION_ENABLE_JTAG) )
    {
        // software disable of JTAG
        // From http://code.google.com/p/micropendous/wiki/Micropendous3
        // note the JTD bit must be written twice within 4 clock cycles to disable JTAG
        // you must also set the IVSEL bit at the same time, which requires IVCE to be set first
        // port pull-up resistors are enabled - PUD(Pull Up Disable) = 0
        MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD);
        MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);
    }

    //byte bBusPowered = pApplicationHdr->nOptions & DEVICE_OPTION_BUS_POWERED;
    nUseStatusLEDs = pApplicationHdr->nOptions & DEVICE_OPTION_USE_USBKEY_LEDS;
    // not used yet byte bHIDDebug = pApplicationHdr->nOptions & DEVICE_OPTION_HID_DEBUG;
    nSerialDebugLevel = pApplicationHdr->nDebugLevel;
    byte b5Volt = pApplicationHdr->nOptions & DEVICE_OPTION_5V;
    pReportLengths = pApplicationHdr->OutputReportLength;
    pApplicationData = (byte *)pApplicationHdr + sizeof(struct SApplicationHeader);
    ReadBufferPtr = 0;
    ReadState = ReadReportId;
    bInitialised = false;
    nLastPacketTime = 0;
    bForcePacket = false;

    if ( pApplicationHdr->nPowerPort != 0xFF )
    {
	SetDDR( pApplicationHdr->nPowerPort, DDR_OUTPUT );
	SetIOBit( pApplicationHdr->nPowerPort );
    }

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

    if ( nSerialDebugLevel > 0 )
    {
	#if F_CPU == 8000000
	    UART1_Init( 12 );		// Fixed at 38400,8,n,1
	#elif F_CPU == 16000000
	    UART1_Init( 25 );		// Fixed at 38400,8,n,1
	#endif
    }

    sei();

    if ( nSerialDebugLevel > 0 )
    {
	UART1_Send_P( PSTR("USB HID v1.0 Initialised (") );
	UART1_SendHex( nReason );
	UART1_Send_P( PSTR(")\r\n") );
	if ( pApplicationHdr->nPowerPort != 0xFF )
	{
	    UART1_Send_P( PSTR("Power=P") );
	    UART1_SendChar( 'A' + (pApplicationHdr->nPowerPort >> 3) );
	    UART1_SendChar( '0' + (pApplicationHdr->nPowerPort % 8) );
	    UART1_SendCRLF( );
	}
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
    {
	USB_Init( USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED);
    }
    else
    {
	USB_Init( USB_DEVICE_OPT_FULLSPEED);
    }

    /* Scheduling - routine never returns, so put this last in the main function */
    for (;;)
    {
	USB_USBTask();
	GenericHIDProcessing();
    }
}

/*
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
*/


static bool SendInputReport(bool bForcePacket )
{
    bool bSent = false;
    byte nLen = 0;
    byte buf[JOYSTICK_IN_EPSIZE];
    ReadControls( 1, pApplicationData, buf, &nLen );

    if ( ( (milliseconds_counter16 - nLastPacketTime) > MINIMUM_PACKET_TIME  && memcmp( buf, LastSendBuffer, nLen ) != 0 ) ||
	 bForcePacket )
    {
	if (USB_DeviceState != DEVICE_STATE_Unattached)
	{
	    Endpoint_SelectEndpoint(JOYSTICK_IN_EPNUM);

	    if (Endpoint_IsReadWriteAllowed() && Endpoint_IsINReady() )
	    {
		Endpoint_Write_Byte(1);			// Report ID - input report is always 1
		if ( nSerialDebugLevel > 10 )
		{
		    UART1_SendHex( 1 );
		    UART1_SendChar(' ');
		}

		for ( byte i = 0; i < nLen; i++ )
		{
		    Endpoint_Write_Byte(buf[i]);
		    if ( nSerialDebugLevel > 10 )
		    {
			UART1_SendHex( buf[i] );
			UART1_SendChar(' ');
		    }
		}
		if ( nSerialDebugLevel > 10 )
		{
		    UART1_SendCRLF();
		}
		Endpoint_ClearIN();

		memcpy( LastSendBuffer, buf, nLen );
		nLastPacketTime = milliseconds_counter16;
	    }
	}
    }

    return bSent;
}


void EVENT_USB_Device_UnhandledControlRequest(void)
{
    if (!(Endpoint_IsSETUPReceived()))
	return;

    if (USB_ControlRequest.wIndex != JOYSTICK_INTERFACE_NUMBER)
	return;

    const uint8_t Request = USB_ControlRequest.bRequest;
    const uint8_t RequestType = USB_ControlRequest.bmRequestType;

    /* Process specific control requests */
    if ( nSerialDebugLevel > 10 )
    {
	UART1_Send_P(PSTR("UCP="));
	UART1_SendHex( Request );
	UART1_Send_P(PSTR(","));
	UART1_SendHex( RequestType );
	UART1_SendCRLF();
    }

    switch (USB_ControlRequest.bRequest)
    {
	case REQ_GetReport:
	    if (RequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
	    {
		Endpoint_ClearSETUP();	

		byte nReportId = USB_ControlRequest.wValue & 0xFF;
		byte type = (USB_ControlRequest.wValue >> 8) - 1;
		int nEndPoint = USB_ControlRequest.wIndex;
		int nRequestLen = USB_ControlRequest.wLength;

		if ( nSerialDebugLevel > 10 )
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

		if ( type == REPORT_ITEM_TYPE_Feature )				    // Feature report requested - this will be for LCD info
		{
		    // Write Feature report directly to usb port
		    WriteFeatureReport( nReportId, pApplicationData );
		    Endpoint_ClearIN();

		    while ( !Endpoint_IsOUTReceived() )
		    {
			if (USB_DeviceState == DEVICE_STATE_Unattached)
			    return;
		    }
		    Endpoint_ClearOUT();
		}
		else if ( type == REPORT_ITEM_TYPE_In && nReportId == 1 )	    // we only have 1 input report
		{
		    SendInputReport( true );
		}
		else
		{
		    Endpoint_ClearIN();
		}
	    }
	    break;

	case REQ_SetReport:
	    if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
	    {
		if ( nSerialDebugLevel > 10 )
		{
		    UART1_Send_P(PSTR("SET_IDLE"));
		    UART1_SendCRLF();
		}
		//Endpoint_ClearSETUP();

		//uint16_t ReportOUTSize = USB_ControlRequest.wLength;
		//uint8_t  ReportOUTData[ReportOUTSize];
		//uint8_t  ReportID = (USB_ControlRequest.wValue & 0xFF);

		//Endpoint_Read_Control_Stream_LE(ReportOUTData, ReportOUTSize);
		//Endpoint_ClearIN();

		//CALLBACK_HID_Device_ProcessHIDReport(HIDInterfaceInfo, ReportID, ReportOUTData, ReportOUTSize);
	    }

	    break;
	case REQ_GetProtocol:
	    if ( nSerialDebugLevel > 10 )
	    {
		UART1_Send_P(PSTR("GET_PROTOCOL"));
		UART1_SendCRLF();
	    }
	    break;
	case REQ_SetProtocol:
	    if ( nSerialDebugLevel > 10 )
	    {
		UART1_Send_P(PSTR("SET_PROTOCOL"));
		UART1_SendCRLF();
	    }
	    break;
	case REQ_SetIdle:
	    if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
	    {
		if ( nSerialDebugLevel > 10 )
		{
		    UART1_Send_P(PSTR("SET_IDLE"));
		    UART1_SendCRLF();
		}
	    }

	    break;
	case REQ_GetIdle:
	    if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
	    {		
		if ( nSerialDebugLevel > 10 )
		{
		    UART1_Send_P(PSTR("GET_IDLE"));
		    UART1_SendCRLF();
		}
	    }

	    break;
    }

}

void EVENT_USB_Device_ConfigurationChanged(void)
{
    if ( nSerialDebugLevel > 0 )
    {
	UART1_Send_P(PSTR("\r\nCfgChg "));
	switch ( USB_DeviceState )
	{
	    case DEVICE_STATE_Unattached:   UART1_Send_P(PSTR("Unattached\r\n")); break;
	    case DEVICE_STATE_Powered:	    UART1_Send_P(PSTR("Powered\r\n")); break;
	    case DEVICE_STATE_Default:	    UART1_Send_P(PSTR("Default\r\n")); break;
	    case DEVICE_STATE_Addressed:    UART1_Send_P(PSTR("Addressed\r\n")); break;
	    case DEVICE_STATE_Configured:   UART1_Send_P(PSTR("Configured\r\n")); break;
	    case DEVICE_STATE_Suspended:    UART1_Send_P(PSTR("Suspended\r\n")); break;
	    default:			    UART1_Send_P(PSTR("?\r\n")); break;
	}
    }

    // Allocate the largest endpoint buffers.  We only have two, so even at 64 bytes, double buffered, we only use 256 of 836 bytes
    Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPNUM, EP_TYPE_INTERRUPT,
	ENDPOINT_DIR_IN, JOYSTICK_IN_EPSIZE,
	ENDPOINT_BANK_DOUBLE);

    Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPNUM, EP_TYPE_INTERRUPT,
	ENDPOINT_DIR_OUT, JOYSTICK_OUT_EPSIZE,
	ENDPOINT_BANK_DOUBLE);

    USB_Device_EnableSOFEvents();

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
	if (USB_DeviceState == DEVICE_STATE_Configured)
	    ClearIOBit( pApplicationHdr->nPowerPort );
	else
	    SetIOBit( pApplicationHdr->nPowerPort );
    }
}

void EVENT_USB_Device_Disconnect(void)
{
    if ( pApplicationHdr->nPowerPort != 0xFF )	
    {
	SetIOBit( pApplicationHdr->nPowerPort );
    }
    if ( nSerialDebugLevel > 0 )
    {
	UART1_Send_P(PSTR("\r\nDisconnect\r\n"));
    }
}

void EVENT_USB_Device_StartOfFrame(void)
{
}

void EVENT_USB_Device_Connect(void)
{
    if ( nSerialDebugLevel > 0 )
    {
	UART1_Send_P(PSTR("\r\nConnect\r\n"));
    }
}

static void GenericHIDProcessing(void)
{
    if (USB_DeviceState != DEVICE_STATE_Configured)
	return;

    if ( !bInitialised )
    {
	if ( nSerialDebugLevel > 0 )
	    UART1_Send_P( PSTR("Initialising\r\n") );

	InitControls( pApplicationData );
	bInitialised = true;

	if ( nSerialDebugLevel > 0 )
	    UART1_Send_P( PSTR("Initialisation Complete\r\n") );
    }

    {
	static uint16_t t = 0;
	static uint8_t old = 0;
	t++;
	if ( old != seconds_counter )
	{
	    old = seconds_counter;
	    if ( nSerialDebugLevel > 0 )
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
    if ( !SendInputReport( bForcePacket ) )
	bForcePacket = false;

    Endpoint_SelectEndpoint(JOYSTICK_OUT_EPNUM);
    if (Endpoint_IsOUTReceived() )
    {
	if ( nSerialDebugLevel > 10 )
	{
	    UART1_Send_P( PSTR("Received ") );
	}

	while ( Endpoint_BytesInEndpoint() )
	{
	    byte b = Endpoint_Read_Byte();
	    if ( nSerialDebugLevel > 10 )
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
			if ( nSerialDebugLevel > 10 )
			{
			    UART1_Send_P( PSTR("BytesToRead=") );
			    UART1_SendHex( BytesToRead );
			    UART1_SendChar( ' ' );
			}
			ReadState = ReadData;
		    }
		    else
		    {
			Endpoint_ClearOUT();
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
	Endpoint_ClearOUT();
	if ( nSerialDebugLevel > 10 )
	{
	    UART1_SendCRLF();
	}
    }
}

static void ProcessOutputReport( byte *buf )
{
    if ( nSerialDebugLevel > 10 ) UART1_Send_P( PSTR("Processing Output Report\r\n") );

    if ( *buf == BOOTLOADER_REPORT_ID )
    {				
	// DFU report.  Data is UInt32.  Check the value = 0xDF0DF0DF then restart.
	if ( *((uint32_t *)(buf+1)) == MAGIC_BOOTLOADER_CODE )
	{
	    if ( nSerialDebugLevel > 0 )
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
