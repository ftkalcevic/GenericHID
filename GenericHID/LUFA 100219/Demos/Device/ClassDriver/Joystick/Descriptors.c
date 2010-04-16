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


static void Dump( void* const DescriptorAddr, uint16_t const Size)
{
    return;
    uint16_t nSize = Size;
    byte *pData = (byte *)DescriptorAddr;

    for ( int i = 0; i < nSize; i++ )
    {
	if ( i % 16 == 0 && i != 0 )
	    UART1_SendCRLF();
	UART1_SendHex( pData[i] );
	UART1_SendChar( ' ' );
    }
    UART1_SendCRLF();
}


uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex, void** const DescriptorAddress)
{
    const uint8_t  DescriptorType   = (wValue >> 8);
    const uint8_t  DescriptorNumber = (wValue & 0xFF);

    void*    Address = NULL;
    uint16_t Size    = NO_DESCRIPTOR;

    struct SDynamicHID *pHIDData = (struct SDynamicHID *)DynamicHIDData;

    switch (DescriptorType)
    {
	case DTYPE_Device:
	    Address = DynamicHIDData + pHIDData->nDeviceDescriptorOffset;
	    Size    = pHIDData->nDeviceDescriptorLength;
	    if ( bSerialDebug )
	    {
		UART1_Send_P( PSTR("Dev ") );
		UART1_SendInt( pHIDData->nDeviceDescriptorOffset );
		UART1_Send_P( PSTR(" ") );
		UART1_SendInt( pHIDData->nDeviceDescriptorLength );
		UART1_SendCRLF();
	    }
	    break;

	case DTYPE_Configuration:
	    Address = DynamicHIDData + pHIDData->nConfigDescriptorOffset;
	    Size    = pHIDData->nConfigDescriptorLength;
	    if ( bSerialDebug )
	    {
		UART1_Send_P( PSTR("Config ") );
		UART1_SendInt( pHIDData->nConfigDescriptorOffset );
		UART1_Send_P( PSTR(" ") );
		UART1_SendInt( pHIDData->nConfigDescriptorLength);
		UART1_SendCRLF();			
	    }
	    break;

	case DTYPE_String:
	    // TODO - error check string table length
	    Address = DynamicHIDData + pHIDData->nStringIndex0Offset[DescriptorNumber];
	    Size    = *(byte *)(Address);
	    if ( bSerialDebug )
	    {
		UART1_Send_P( PSTR("Str ") );
		UART1_SendInt( DescriptorNumber );
		UART1_Send_P( PSTR(" ") );
		UART1_SendInt( pHIDData->nStringIndex0Offset[DescriptorNumber] );
		UART1_Send_P( PSTR(" ") );
		UART1_SendInt( Size);
		UART1_SendCRLF();			
	    }
	    break;

	case DTYPE_HID:
	    Address = DynamicHIDData + pHIDData->nHIDDescriptorOffset;
	    Size    = pHIDData->nHIDDescriptorLength;
	    if ( bSerialDebug )
	    {
		UART1_Send_P( PSTR("HID ") );
		UART1_SendInt( pHIDData->nHIDDescriptorOffset );
		UART1_Send_P( PSTR(" ") );
		UART1_SendInt( pHIDData->nHIDDescriptorLength);
		UART1_SendCRLF();			
	    }
	    break;

	case DTYPE_Report:
	    Address = DynamicHIDData + pHIDData->nReportDescriptorOffset;
	    Size    = pHIDData->nReportDescriptorLength;
	    if ( bSerialDebug )
	    {
		UART1_Send_P( PSTR("Report ") );
		UART1_SendInt( pHIDData->nReportDescriptorOffset );
		UART1_Send_P( PSTR(" ") );
		UART1_SendInt( pHIDData->nReportDescriptorLength);
		UART1_SendCRLF();			
	    }
	    break;
    }
    if ( bSerialDebug && Size > 0 )
    {
	Dump( Address, Size );
    }

    *DescriptorAddress = Address;
    return Size;
}
