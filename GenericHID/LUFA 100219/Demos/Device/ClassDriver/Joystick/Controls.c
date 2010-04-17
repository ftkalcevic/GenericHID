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



#include "Common.h"
#include "Serial.h"
#include "Controls.h"
#include "Potentiometer.h"
#include "Switch.h"
#include "RotarySwitch.h"
#include "KeyMatrix.h"
#include "DigitalEncoder.h"
#include "LED.h"
#include "BicolourLED.h"
#include "TricolourLED.h"
#include "LCD.h"
#include "DirSwitch.h"
#include "Counter.h"
#include "pwm.h"
#include "rgb.h"
#include "LCD_SPI.h"

void InitControls( byte *pData )
{
    while ( *pData != 0 )
    {
	struct SControlHeader *hdr = (struct SControlHeader *)pData;

	switch ( hdr->Type )
	{
	    case Potentiometer:
		InitPotentiometer( (struct SPotentiometerControl *)pData );
		break;
	    case Switch:
		InitSwitch( (struct SSwitchControl *)pData );
		break;
	    case RotarySwitch:
		InitRotarySwitch( (struct SRotarySwitchControl *)pData );
		break;
	    case KeyMatrix:
		InitKeyMatrix( (struct SKeyMatrixControl *)pData );
		break;
	    case DigitalEncoder:
		InitDigitalEncoder( (struct SDigitalEncoderControl *)pData );
		break;
	    case LED:
		InitLED( (struct SLEDControl *)pData );
		break;
	    case BicolourLED:
		InitBicolourLED( (struct SBicolourLEDControl *)pData );
		break;
	    case TricolourLED:
		InitTricolourLED( (struct STricolourLEDControl *)pData );
		break;
	    case RGBLED:
		InitRGB( (struct SRGBLEDControl *)pData );
		break;
	    case LCD:
		InitLCD( (struct SLCDControl *)pData );
		break;
	    case DirectionalSwitch:
		InitDirSwitch( (struct SDirSwitchControl *)pData );
		break;
	    case Counter:
		InitCounter( (struct SCounterControl *)pData );
		break;
	    case PWMOutput:
		InitPWM( (struct SPWMControl *)pData );
		break;
	    case LCD_SPI:
		InitLCD_SPI( (struct SLCDSPIControl *)pData );
		break;
	    default:
		if ( nSerialDebugLevel > 0 )
		{
		    UART1_Send_P(PSTR("Got unexpected type in InitControls - 0x"));
		    UART1_SendHex(*pData);
		    UART1_SendCRLF();
		}
		return;
	}
	pData += hdr->Length;
    }
}



void ReadControls( byte nReportId, byte *pData, byte *pReportBuffer, byte *nLength )
{
    byte *pReport = pReportBuffer;
    byte nOutMask = 1;
    *pReport = 0;

    while ( *pData != 0 )
    {
	struct SControlHeader *header = (struct SControlHeader *)pData;
	if ( nReportId == header->ReportId || ( nReportId >= header->ReportId && nReportId <= header->ReportIdMax ) )
	{
	    switch ( header->Type )
	    {
		case Potentiometer:
		    ReadPotentiometer( (struct SPotentiometerControl *)pData,  &pReport, &nOutMask );
		    break;
		case Switch:
		    ReadSwitch( (struct SSwitchControl *)pData,  &pReport, &nOutMask );
		    break;
		case RotarySwitch:
		    ReadRotarySwitch( (struct SRotarySwitchControl *)pData,  &pReport, &nOutMask );
		    break;
		case KeyMatrix:
		    ReadKeyMatrix( (struct SKeyMatrixControl *)pData,  &pReport, &nOutMask );
		    break;
		case DigitalEncoder:
		    ReadDigitalEncoder( (struct SDigitalEncoderControl *)pData,  &pReport, &nOutMask );
		    break;
		case DirectionalSwitch:
		    ReadDirSwitch( (struct SDirSwitchControl *)pData,  &pReport, &nOutMask );
		    break;
		case Counter:
		    ReadCounter( (struct SCounterControl *)pData, &pReport, &nOutMask );
		    break;
	    }
	}
	pData += header->Length;
    }
    *nLength = pReport - pReportBuffer + 1;
}

void PackData16( byte **_pBuf, byte *_nOutMask, uint16_t nValue, byte nBits )
{
    byte *pBuf = *_pBuf;
    byte nOutMask = *_nOutMask;

    uint16_t nDataMask = 1;
    for ( byte i = 0; i < nBits; i++ )
    {
	if ( nOutMask == 0 )	// do this here so the pointer doesn't get incremented past the end.
	{
	    nOutMask = 1;
	    pBuf++;
	    *pBuf = 0;
	}
	if ( nValue & nDataMask )
	    *pBuf |= nOutMask;
	nOutMask <<= 1;
	nDataMask <<= 1;
    }

    *_pBuf = pBuf;
    *_nOutMask = nOutMask;
}

// Output controls
void WriteControls( byte nReportId, byte *pData, byte *pReportBuffer )
{
    byte *pReport = pReportBuffer;
    byte nOutMask = 1;

    while ( *pData != 0 )
    {
	struct SControlHeader *header = (struct SControlHeader *)pData;
	if ( nReportId == header->ReportId || ( nReportId >= header->ReportId && nReportId <= header->ReportIdMax ) )
	{
	    switch ( header->Type )
	    {
		case LED:
		    WriteLED( (struct SLEDControl *)pData,  &pReport, &nOutMask );
		    break;
		case BicolourLED:
		    WriteBicolourLED( (struct SBicolourLEDControl *)pData,  &pReport, &nOutMask );
		    break;
		case TricolourLED:
		    WriteTricolourLED( (struct STricolourLEDControl *)pData,  &pReport, &nOutMask );
		    break;
		case RGBLED:
		    WriteRGB( (struct SRGBLEDControl *)pData,  &pReport, &nOutMask );
		    break;
		case LCD:
		    WriteLCD( (struct SLCDControl *)pData, nReportId, &pReport, &nOutMask );
		    break;
		case PWMOutput:
		    WritePWM( (struct SPWMControl *)pData, &pReport, &nOutMask );
		    break;
		case LCD_SPI:
		    WriteLCD_SPI( (struct SLCDSPIControl *)pData, nReportId, &pReport, &nOutMask );
		    break;
	    }
	}
	pData += header->Length;
    }
}




uint16_t ReadPackData16( byte **_pBuf, byte *_nOutMask, byte nBits )
{
    uint16_t nRet = 0;

    byte *pBuf = *_pBuf;
    byte nOutMask = *_nOutMask;

    uint16_t nDataMask = 1;
    for ( byte i = 0; i < nBits; i++ )
    {
	if ( nOutMask == 0 )	// do this here so the pointer doesn't get incremented past the end.
	{
	    nOutMask = 1;
	    pBuf++;
	}
	if ( *pBuf & nOutMask )
	    nRet |= nDataMask;
	nOutMask <<= 1;
	nDataMask <<= 1;
    }

    *_pBuf = pBuf;
    *_nOutMask = nOutMask;

    return nRet;
}


// Output requested feature report directly to usb
void WriteFeatureReport( byte nReportId, byte *pData )
{
    while ( *pData != 0 )
    {
	struct SControlHeader *header = (struct SControlHeader *)pData;
	if ( nReportId == header->ReportId || ( nReportId >= header->ReportId && nReportId <= header->ReportIdMax ) )
	{
	    switch ( header->Type )
	    {
		case LCD:
		    SendFeatureAttributeReportLCD( (struct SLCDControl *)pData );
		    break;
		case LCD_SPI:
		    SendFeatureAttributeReportLCD_SPI( (struct SLCDSPIControl *)pData );
		    break;
	    }
	}
	pData += header->Length;
    }
}

/*
byte GetPINBit( byte nPort, byte nPin )
{
byte n = 0;
switch ( nPort )
{
case PortA:
n = PINA; break;
case PortB:
n = PINB; break;
case PortC:
n = PINC; break;
case PortD:
n = PIND; break;
case PortE:
n = PINE; break;
case PortF:
n = PINF; break;
}
if ( n & (1<<nPin))
return 1;
else
return 0;
}
*/

#define SET_BIT( port, nPin, n )	{ if ( n ) port |= (1<<nPin); else port &= ~(1<<nPin); }

void SetDDRBit( byte nPort, byte nPin, byte n )
{
    switch ( nPort )
    {
    case PortA:
	SET_BIT( DDRA, nPin, n ); break;
    case PortB:
	SET_BIT( DDRB, nPin, n ); break;
    case PortC:
	SET_BIT( DDRC, nPin, n ); break;
    case PortD:
	SET_BIT( DDRD, nPin, n ); break;
    case PortE:
	SET_BIT( DDRE, nPin, n ); break;
    case PortF:
	SET_BIT( DDRF, nPin, n ); break;
    }
}

/*
void SetPORTBit( byte nPort, byte nPin, byte n )
{
switch ( nPort )
{
case PortA:
SET_BIT( PORTA, nPin, n ); break;
case PortB:
SET_BIT( PORTB, nPin, n ); break;
case PortC:
SET_BIT( PORTC, nPin, n ); break;
case PortD:
SET_BIT( PORTD, nPin, n ); break;
case PortE:
SET_BIT( PORTE, nPin, n ); break;
case PortF:
SET_BIT( PORTF, nPin, n ); break;
}
}
*/
