#ifndef _DEVICECONFIG_H_
#define _DEVICECONFIG_H_

#if defined(_WIN32) || defined(_LINUX)
#pragma pack(push,1)

#include "types.h"

#endif

enum ControlType
{
    None = 0,
    Potentiometer = 1,
    Switch =2,
    RotarySwitch = 3,
    KeyMatrix = 4,
    DigitalEncoder = 5,
    LED = 6,
    BicolourLED = 7,
    TricolourLED = 8, 
    LCD = 9,
    DirectionalSwitch = 10,
    Counter = 11,
    AnalogEncoder = 12,
    PWMOutput = 13,
    RGBLED = 14,
};

struct SControlHeader
{
    byte Type;
    byte Length;
    byte ReportId;
};


enum PortIDs
{
    PortA = 0,
    PortB,
    PortC,
    PortD,
    PortE,
    PortF,
};


struct SAnalogEncoderControl
{
    struct SControlHeader hdr;
    byte PortA;
    byte PortB;
    byte Options;
	#define AE_PULLUPA     0
	#define AE_PULLUPB     1
	#define AE_DEBOUNCE    2
};


struct SBicolourLEDControl
{
    struct SControlHeader hdr;
    byte PortA;
    byte PortB;
};

struct SCounterControl
{
    struct SControlHeader hdr;
    uint16_t Period;
    byte Bits;
};

struct SDigitalEncoderControl
{
    struct SControlHeader hdr;
    byte PortA;
    byte PortB;
};

enum DirSwitchType
{
    DS_NS = 0,
    DS_WE,
    DS_4way,
    DS_8way
};

struct SDirSwitchControl
{
    struct SControlHeader hdr;
    byte Type;
    byte Ports;
    byte Options;
	#define DSW_PULLUP      0
	#define DSW_DEBOUNCE    1
    struct _SDirSwitchData
    {
	byte Port;
	byte Debounce;
    } Switches[0];
};

#define DIRSW_PORTN	0
#define DIRSW_PORTS 	1
#define DIRSW_PORTE 	2
#define DIRSW_PORTW 	3
#define DIRSW_PORTNE	4
#define DIRSW_PORTNW	5
#define DIRSW_PORTSE	6
#define DIRSW_PORTSW	7


struct SKeyMatrixControl
{
    struct SControlHeader hdr;
    byte Rows;
    byte Cols;
    byte Options;
	#define KM_PULLUP      0
	#define KM_DEBOUNCE    1
    byte Data[0];			// row ports (Rows)
					// column ports (Cols)
					// debounce buffer (Cols * Rows)
};


struct SLCDControl
{
    struct SControlHeader hdr;
    byte nRows;
    byte nColumns;
    byte b8Bit;
    byte nPortRS;
    byte nPortRW;
    byte nPortE;
    byte nPortD0;
    byte nPortD1;
    byte nPortD2;
    byte nPortD3;
    byte nPortD4;
    byte nPortD5;
    byte nPortD6;
    byte nPortD7;
    byte RowAddr[4];		// Start of each row
};


struct SLEDControl
{
    struct SControlHeader hdr;
    byte Port;
    byte Options;
	#define LED_SINK      0
};

struct SPotentiometerControl
{
    struct SControlHeader hdr;
    byte Port;
    byte Bits;
};

struct SPWMControl
{
    struct SControlHeader hdr;
    byte Port;
    unsigned short Period;
    byte Resolution;
};

struct SRGBLEDControl
{
    struct SControlHeader hdr;
    byte PortR;
    byte PortG;
    byte PortB;
    byte Options;
	#define RGB_SINK      0
};



struct SRotarySwitchControl
{
    struct SControlHeader hdr;
    byte PinCount;
    byte ReportSize;
    byte Options;
	#define RSW_PULLUP      0
	#define RSW_DEBOUNCE    1
	#define RSW_ENCODED     2
    byte LastValue;
    struct _SRotarySwitchPin
    {
	byte Port;
	byte Bit;
	byte Debounce;
    } Pins[0];
};


struct SSwitchControl
{
    struct SControlHeader hdr;
    byte Port;
    byte Options;
	#define SW_PULLUP      0
	#define SW_DEBOUNCE    1
    byte Debounce;
};

struct STricolourLEDControl
{
    struct SControlHeader hdr;
    byte PortA;
    byte PortB;
    byte Options;
	#define TRI_SINK      0
};





struct SDynamicHID
{
    uint16_t nBlockLength;
    uint16_t nStringCount;
    uint16_t nReportDescriptorOffset;
    uint16_t nReportDescriptorLength;
    uint16_t nDeviceDescriptorOffset;
    uint16_t nDeviceDescriptorLength;
    uint16_t nConfigDescriptorOffset;
    uint16_t nConfigDescriptorLength;
    uint16_t nHIDDescriptorOffset;
    uint16_t nHIDDescriptorLength;
    uint16_t nApplicationDataOffset;
    uint16_t nStringIndex0Offset[0];
};

#define MAX_READ_BUFFER		64
#define MAX_REPORTS		10

#define DEVICE_OPTION_BUS_POWERED	0x01
#define DEVICE_OPTION_USE_USBKEY_LED1	0x02
#define DEVICE_OPTION_USE_USBKEY_LED2	0x04
#define	DEVICE_OPTION_USE_USBKEY_LEDS	(DEVICE_OPTION_USE_USBKEY_LED1|DEVICE_OPTION_USE_USBKEY_LED2)
#define DEVICE_OPTION_HID_DEBUG		0x08
#define DEVICE_OPTION_SERIAL_DEBUG	0x10
#define DEVICE_OPTION_5V		0x20

struct SApplicationHeader
{
    byte nOptions;
    byte ReportLength[MAX_REPORTS];
};


#define MAGIC_BOOTLOADER_CODE 0xDF0DF0DF
#define BOOTLOADER_REPORT 3
#define MAX_HID_DATA	4096				// 4k of eeprom

#if defined(_WIN32) || defined(_LINUX)
#pragma pack(pop)
#endif

#endif
