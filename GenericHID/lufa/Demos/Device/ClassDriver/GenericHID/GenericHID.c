#include <avr/eeprom.h>
#include <avr/wdt.h>
#include "Common.h"
#include "Serial.h"
#include "GenericHID.h"
#include "Controls.h"

static void ProcessOutputReport( byte *buf );

//! Declare function pointer to USB bootloader entry point   - todo, use macros, don't hardcode.
void (*start_bootloader) (void)=(void (*)(void))0xf000;

byte DynamicHIDData[MAX_HID_DATA];			// buffer to copy eeprom to.
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



/** LUFA HID Class driver interface configuration and state information. This structure is
*  passed to all HID Class driver functions, so that multiple instances of the same class
*  within a device can be differentiated from one another.
*/
USB_ClassInfo_HID_Device_t Generic_HID_Interface =
{
    .Config =
    {
	.InterfaceNumber         = 0,

	.ReportINEndpointNumber  = GENERIC_IN_EPNUM,
	.ReportINEndpointSize    = GENERIC_EPSIZE,

	.PrevReportINBuffer      = PrevHIDReportBuffer,
	.PrevReportINBufferSize  = sizeof(PrevHIDReportBuffer),
    },
};

/** Main program entry point. This routine contains the overall program flow, including initial
*  setup of all components and the main program loop.
*/
int main(void)
{
    SetupHardware();

    LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

    for (;;)
    {
	HID_Device_USBTask(&Generic_HID_Interface);
	USB_USBTask();
    }
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Hardware Initialization */
    LEDs_Init();
    USB_Init();

    /* Millisecond timer initialization, with output compare interrupt enabled for the idle timing */
    OCR0A  = ((F_CPU / 64) / 1000);
    TCCR0A = (1 << WGM01);
    TCCR0B = ((1 << CS01) | (1 << CS00));
    TIMSK0 = (1 << OCIE0A);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    LEDs_SetAllLEDs(LEDMASK_USB_READY);

    if (!(HID_Device_ConfigureEndpoints(&Generic_HID_Interface)))
	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Unhandled Control Request event. */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
    HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** ISR to keep track of each millisecond interrupt, for determining the HID class idle period remaining when set. */
ISR(TIMER0_COMPA_vect, ISR_BLOCK)
{
    HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
*
*  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
*  \param[in,out] ReportID  Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
*  \param[out] ReportData  Pointer to a buffer where the created report should be stored
*  \param[out] ReportSize  Number of bytes written in the report (or zero if no report is to be sent
*
*  \return Boolean true to force the sending of the report, false to let the library determine if it needs to be sent
*/
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, uint8_t* const ReportID,
					 void* ReportData, uint16_t* ReportSize)
{
    if (HIDReportEcho.ReportID)
	*ReportID = HIDReportEcho.ReportID;

    memcpy(ReportData, HIDReportEcho.ReportData, HIDReportEcho.ReportSize);

    *ReportSize = HIDReportEcho.ReportSize;
    return true;
}

/** HID class driver callback function for the processing of HID reports from the host.
*
*  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
*  \param[in] ReportID  Report ID of the received report from the host
*  \param[in] ReportData  Pointer to a buffer where the created report has been stored
*  \param[in] ReportSize  Size in bytes of the received HID report
*/
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, const uint8_t ReportID,
					  const void* ReportData, const uint16_t ReportSize)
{
    HIDReportEcho.ReportID   = ReportID;
    HIDReportEcho.ReportSize = ReportSize;
    memcpy(HIDReportEcho.ReportData, ReportData, ReportSize);
}
