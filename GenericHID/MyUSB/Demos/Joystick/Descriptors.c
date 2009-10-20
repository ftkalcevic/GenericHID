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

#include "Common.h"
#include "Serial.h"
#include "Descriptors.h"
#include "Joystick.h"

#if 0
uint8_t JoystickReport[] PROGMEM =
{
		//0x05, 0x01,          /* Usage Page (Generic Desktop)                       */
		//0x09, 0x04,          /* Usage (Joystick)                                   */
		//0xa1, 0x01,          /* Collection (Application)                           */
		//0x09, 0x01,          /*   Usage (Pointer)                                  */
		//0xa1, 0x02,          /*   Collection (Logical)                             */

		//0x85, 0x01,                    // REPORT_ID (1)
		//0x05, 0x01,          /*     Usage Page (Generic Desktop)                   */
		//0x09, 0x30,          /*     Usage (X)                                      */
		//0x09, 0x31,          /*     Usage (Y)                                      */
		//0x15, 0x9c,          /*     Logical Minimum (-100)                         */
		//0x25, 0x64,          /*     Logical Maximum (100)                          */
		//0x75, 0x08,          /*     Report Size (8)                                */
		//0x95, 0x02,          /*     Report Count (2)                               */
		//0x81, 0x82,          /*     Input (Data, Variable, Absolute, Volatile)     */

		//0x05, 0x09,          /*     Usage Page (Button)                            */
		//0x09, 0x02,          /*     Usage (Button 2)                               */
		//0x09, 0x01,          /*     Usage (Button 1)                               */
		//0x15, 0x00,          /*     Logical Minimum (0)                            */
		//0x25, 0x01,          /*     Logical Maximum (1)                            */
		//0x75, 0x01,          /*     Report Size (1)                                */
		//0x95, 0x02,          /*     Report Count (2)                               */
		//0x81, 0x02,          /*     Input (Data, Variable, Absolute)               */
		//0x75, 0x06,          /*     Report Size (6)  - Pad the rest of the report packet */
		//0x95, 0x01,          /*     Report Count (1)                               */
		//0x81, 0x01,          /*     Input (Constant)                               */

		////0x05, 0x08,			/*		Usage Page (Page# for LEDs),					*/
		////0x19, 0x01,			/*		Usage Minimum (1),								*/
		////0x29, 0x05,			/*		Usage Maximum (5),								*/
		////0x91, 0x02,			/*		Output (Data, Variable, Absolute), ;LED report	*/
		////0x95, 0x01,			/*		Report Count (1),								*/
		////0x75, 0x03,			/*		Report Size (3),								*/
		////0x91, 0x01,			/*		Output (Constant), ;LED report padding			*/
		////0x95, 0x06,			/*		Report Count (6),								*/
		////0x75, 0x08,			/*		Report Size (8),								*/

  //  0x85, 0x01,                    // REPORT_ID (1)
  //  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  //  0x09, 0x01,                    // USAGE (Pointer)
  //  0x79, 0x04,                    // STRING_INDEX (4)
  //  0x26, 0xff, 0x00,              // LOGICAL_MAXIMUM (255)
  //  0x15, 0x00,                    // LOGICAL_MINIMUM (0)
  //  0x95, 0x01,                    // REPORT_COUNT (1)
  //  0x75, 0x08,                    // REPORT_SIZE (8)
  //  0x91, 0x02,                    // OUTPUT (Data,Var,Abs)

  //  0x85, 0x02,                    // REPORT_ID (2)
  //  0x09, 0x32,                    // USAGE (Z)
  //  0x79, 0x05,                    // STRING_INDEX (5)
  //  0x95, 0x01,                    // REPORT_COUNT (1)
  //  0x75, 0x08,                    // REPORT_SIZE (8)
  //  0x91, 0x02,                    // OUTPUT (Data,Var,Abs)

  //  0x85, 0x01,                    // REPORT_ID (1)
  //  0x75, 0x08,                    // REPORT_SIZE (8)
  //  0x95, 0x02,                    // REPORT_COUNT (2)
  //  0xb1, 0x03,                    // FEATURE (Cnst,Var,Abs)

		//0xc0,                /*   End Collection                                   */
		//0xc0                 /* End Collection                                     */


    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //   USAGE (X)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x03,              //   LOGICAL_MAXIMUM (1023)
    0x75, 0x0a,                    //   REPORT_SIZE (10)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x79, 0x01,                    //   STRING_INDEX (1)
    0x81, 0x22,                    //   INPUT (Data,Var,Abs,NPrf)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x06,                    //   REPORT_SIZE (6)
    0x81, 0x23,                    //   INPUT (Cnst,Var,Abs,NPrf)
    0xc0                           // END_COLLECTION

};

USB_Descriptor_Device_t DeviceDescriptor PROGMEM =
{
	Header:                 {Size: sizeof(USB_Descriptor_Device_t), Type: DTYPE_Device},
		
	USBSpecification:       0x0101,
	Class:                  0x00,
	SubClass:               0x00,
	Protocol:               0x00,
				
	Endpoint0Size:          ENDPOINT_CONTROLEP_SIZE,
		
	VendorID:               0x0000,
	ProductID:              USB_PRODUCT_ID('J', 'S'),
	ReleaseNumber:          0x0000,
		
	ManafacturerStrIndex:   0x01,
	ProductStrIndex:        0x02,
	SerialNumStrIndex:      0x03,
		
	NumberOfConfigurations: 1
};
	
USB_Descriptor_Configuration_t ConfigurationDescriptor PROGMEM =
{
	Config:
		{
			Header:                 {Size: sizeof(USB_Descriptor_Configuration_Header_t), Type: DTYPE_Configuration},

			TotalConfigurationSize: sizeof(USB_Descriptor_Configuration_t),
			TotalInterfaces:        1,
				
			ConfigurationNumber:    1,
			ConfigurationStrIndex:  NO_DESCRIPTOR_STRING,
				
			ConfigAttributes:       (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),
			
			MaxPowerConsumption:    USB_CONFIG_POWER_MA(100)
		},
		
	Interface:
		{
			Header:                 {Size: sizeof(USB_Descriptor_Interface_t), Type: DTYPE_Interface},

			InterfaceNumber:        JOYSTICK_INTERFACE_NUMBER,
			AlternateSetting:       JOYSTICK_INTERFACE_ALTERNATE,
			
			TotalEndpoints:         JOYSTICK_INTERFACE_ENDPOINTS,
				
			Class:                  JOYSTICK_INTERFACE_CLASS,
			SubClass:               JOYSTICK_INTERFACE_SUBCLASS,
			Protocol:               JOYSTICK_INTERFACE_PROTOCOL,
				
			InterfaceStrIndex:      NO_DESCRIPTOR_STRING
		},

	JoystickHID:
		{
			Header:                 {Size: sizeof(USB_Descriptor_HID_t), Type: DTYPE_HID},
									 
			HIDSpec:          		0x1001,
			CountryCode:      		0x00,
			TotalHIDDescriptors:    0x01,
			HIDReportType:    		0x22,
			HIDReportLength:        sizeof(JoystickReport)
		},

	JoystickEndpoint1:
		{
			Header:                 {Size: sizeof(USB_Descriptor_Endpoint_t), Type: DTYPE_Endpoint},
										 
			EndpointAddress:        (ENDPOINT_DESCRIPTOR_DIR_IN | JOYSTICK_IN_EPNUM),
			Attributes:       		EP_TYPE_INTERRUPT,
			EndpointSize:           JOYSTICK_EPSIZE,
			PollingIntervalMS:		0x02
		},

	JoystickEndpoint2:
		{
			Header:                 {Size: sizeof(USB_Descriptor_Endpoint_t), Type: DTYPE_Endpoint},
										 
			EndpointAddress:        (ENDPOINT_DESCRIPTOR_DIR_OUT | JOYSTICK_OUT_EPNUM),
			Attributes:       		EP_TYPE_INTERRUPT,
			EndpointSize:           JOYSTICK_EPSIZE,
			PollingIntervalMS:		0x02
		}	
};

USB_Descriptor_Language_t LanguageString PROGMEM =
{
	Header:                 {Size: sizeof(USB_Descriptor_Language_t), Type: DTYPE_String},
		
	LanguageID:             LANGUAGE_ID_ENG
};

USB_Descriptor_String_t ManafacturerString PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(11), Type: DTYPE_String},
		
	UnicodeString:          {'D','e','a','n',' ','C','a','m','e','r','a'}
};

USB_Descriptor_String_t ProductString PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(19), Type: DTYPE_String},
		
	UnicodeString:          {'M','y','U','S','B',' ','J','o','y','s','t','i','c','k',' ','D','e','m','o'}
};

USB_Descriptor_String_t SerialNumberString PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(13), Type: DTYPE_String},
		
	UnicodeString:          {'0','.','0','.','0','.','0','.','0','.','0','.','0'}
};

USB_Descriptor_String_t DeviceStringDesc PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(10), Type: DTYPE_String},
		
	UnicodeString:          {'M','y',' ','P','o','i','n','t','e','r'}
};

USB_Descriptor_String_t DeviceStringDesc2 PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(7), Type: DTYPE_String},
		
	UnicodeString:          {'A','n','o','t','h','e','r'}
};
#endif

//bool USB_GetDescriptor(const uint8_t Type, const uint8_t Index,
//                       void** const DescriptorAddr, uint16_t* const Size)
//{
//	switch (Type)
//	{
//		case DTYPE_Device:
//			*DescriptorAddr = (void*)&DeviceDescriptor;
//			*Size           = sizeof(USB_Descriptor_Device_t);
//			return true;
//		case DTYPE_Configuration:
//			*DescriptorAddr = (void*)&ConfigurationDescriptor;
//			*Size           = sizeof(USB_Descriptor_Configuration_t);
//			return true;
//		case DTYPE_String:
//			UART1_Send_P( PSTR("Get String ") );
//			UART1_SendInt( Index );
//			UART1_SendCRLF();
//			switch (Index)
//			{
//				case 0x00:
//					*DescriptorAddr = (void*)&LanguageString;
//					*Size           = sizeof(USB_Descriptor_Language_t);
//					return true;
//				case 0x01:
//					*DescriptorAddr = (void*)&ManafacturerString;
//					*Size           = pgm_read_byte(&ManafacturerString.Header.Size);
//					return true;
//				case 0x02:
//					*DescriptorAddr = (void*)&ProductString;
//					*Size           = pgm_read_byte(&ProductString.Header.Size);
//					return true;
//				case 0x03:
//					*DescriptorAddr = (void*)&SerialNumberString;
//					*Size           = pgm_read_byte(&SerialNumberString.Header.Size);
//					return true;
//				case 0x04:
//					*DescriptorAddr = (void*)&DeviceStringDesc;
//					*Size           = pgm_read_byte(&DeviceStringDesc.Header.Size);
//					return true;
//				case 0x05:
//					*DescriptorAddr = (void*)&DeviceStringDesc2;
//					*Size           = pgm_read_byte(&DeviceStringDesc2.Header.Size);
//					return true;
//			}
//
//			break;
//		case DTYPE_HID:
//			*DescriptorAddr = (void*)&ConfigurationDescriptor.JoystickHID;
//			*Size           = sizeof(USB_Descriptor_HID_t);
//
//			return true;
//		case DTYPE_Report:
//			*DescriptorAddr = (void*)&JoystickReport;
//			*Size           = sizeof(JoystickReport);
//
//			return true;
//	}
//		
//	return false;
//}

static void Dump( void** const DescriptorAddr, uint16_t* const Size)
{
	return;
	uint16_t nSize = *Size;
	byte *pData = (byte *)*DescriptorAddr;

	for ( int i = 0; i < nSize; i++ )
	{
		if ( i % 16 == 0 )
			UART1_SendCRLF();
		UART1_SendHex( pData[i] );
		UART1_SendChar( ' ' );
	}
}


bool USB_GetDescriptor(const uint8_t Type, const uint8_t Index,
                       void** const DescriptorAddr, uint16_t* const Size)
{
	struct SDynamicHID *pHIDData = (struct SDynamicHID *)DynamicHIDData;

	switch (Type)
	{
		case DTYPE_Device:
			*DescriptorAddr = DynamicHIDData + pHIDData->nDeviceDescriptorOffset;
			*Size           = pHIDData->nDeviceDescriptorLength;
			if ( bSerialDebug )
			{
			    UART1_Send_P( PSTR("Get Device Descriptor ") );
			    UART1_SendInt( pHIDData->nDeviceDescriptorOffset );
			    UART1_Send_P( PSTR(" ") );
			    UART1_SendInt( pHIDData->nDeviceDescriptorLength );
			    UART1_SendCRLF();
			    Dump( DescriptorAddr, Size );
			}
			return true;

		case DTYPE_Configuration:
			*DescriptorAddr = DynamicHIDData + pHIDData->nConfigDescriptorOffset;
			*Size           = pHIDData->nConfigDescriptorLength;
			if ( bSerialDebug )
			{
			    UART1_Send_P( PSTR("Get Configuration Descriptor ") );
			    UART1_SendInt( pHIDData->nConfigDescriptorOffset );
			    UART1_Send_P( PSTR(" ") );
			    UART1_SendInt( pHIDData->nConfigDescriptorLength);
			    UART1_SendCRLF();			
			    Dump( DescriptorAddr, Size );
			}
			return true;

		case DTYPE_String:
			*DescriptorAddr = DynamicHIDData + pHIDData->nStringIndex0Offset[Index];
			*Size           = *(byte *)(*DescriptorAddr);
			if ( bSerialDebug )
			{
			    UART1_Send_P( PSTR("Get String ") );
			    UART1_SendInt( Index );
			    UART1_Send_P( PSTR(" ") );
			    UART1_SendInt( pHIDData->nStringIndex0Offset[Index] );
			    UART1_Send_P( PSTR(" ") );
			    UART1_SendInt( *Size);
			    UART1_SendCRLF();			
			    Dump( DescriptorAddr, Size );
			}
			return true;

		case DTYPE_HID:
			*DescriptorAddr = DynamicHIDData + pHIDData->nHIDDescriptorOffset;
			*Size           = pHIDData->nHIDDescriptorLength;
			if ( bSerialDebug )
			{
			    UART1_Send_P( PSTR("Get HID Descriptor ") );
			    UART1_SendInt( pHIDData->nHIDDescriptorOffset );
			    UART1_Send_P( PSTR(" ") );
			    UART1_SendInt( pHIDData->nHIDDescriptorLength);
			    UART1_SendCRLF();			
			    Dump( DescriptorAddr, Size );
			}
			return true;

		case DTYPE_Report:
			*DescriptorAddr = DynamicHIDData + pHIDData->nReportDescriptorOffset;
			*Size           = pHIDData->nReportDescriptorLength;
			if ( bSerialDebug )
			{
			    UART1_Send_P( PSTR("Get Report Descriptor ") );
			    UART1_SendInt( pHIDData->nReportDescriptorOffset );
			    UART1_Send_P( PSTR(" ") );
			    UART1_SendInt( pHIDData->nReportDescriptorLength);
			    UART1_SendCRLF();			
			    Dump( DescriptorAddr, Size );
			}
			return true;
	}
		
	return false;
}
