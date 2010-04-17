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
#include "RotarySwitch.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything

#define DEBOUNCE_COUNTER	10				// 10 ms

void InitRotarySwitch( struct SRotarySwitchControl *pData )
{
    if ( !bInitialised )
    {
    }

    byte bPullup = pData->Options & _BV(RSW_PULLUP);
    for ( byte i = 0; i < pData->PinCount; i++ )
    {
	byte nPort = GET_PORT_ID(pData->Pins[i].Port);
	byte nPin = GET_PORT_PIN(pData->Pins[i].Port);

	// Set DDR - set port to input
	SetDDRBit( nPort, nPin, DDR_INPUT );

	// Set pullup if requested
	if ( bPullup )
	    SetPORTBit( nPort, nPin, 1 );

	pData->Pins[i].Debounce = 0;

	if ( nSerialDebugLevel > 10 )
	{
	    UART1_Send_P( PSTR("Port ") );
	    UART1_SendHex( pData->Pins[i].Port );
	    UART1_Send_P( PSTR(",") );
	    UART1_SendHex( pData->Pins[i].Bit );
	    UART1_SendCRLF();
	}
    }

    bInitialised = true;
}


void ReadRotarySwitch( struct SRotarySwitchControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nValue = 0;
    byte nEncoded = pData->Options & _BV(RSW_ENCODED);
    byte nPullup = pData->Options & _BV(RSW_PULLUP);
    byte nDebounce = pData->Options & _BV(RSW_DEBOUNCE);
    if ( !nEncoded )
	nValue = pData->LastValue;  // For non-encoded, use the last value so we don't get non-selections
    for ( byte i = 0; i < pData->PinCount; i++ )
    {
	//byte nPort = GET_PORT_ID(pData->Pins[i].Port);
	//byte nPin = GET_PORT_PIN(pData->Pins[i].Port);

	//byte bOn = GetPINBit( nPort, nPin );
	byte bOn = GetIOBit( pData->Pins[i].Port );

	if ( nPullup )
	    bOn = 1 - bOn;	

	if ( nDebounce )
	{
	    // if the key has just been pressed, start the debounce counter.  Debounce counter decrements every ms.  Then we take the key.
	    if ( !bOn )
		pData->Pins[i].Debounce = 0;
	    else
	    {
		if ( pData->Pins[i].Debounce & 0x80 )
		{
		    // high bit set - already debounced
		}
		else if ( pData->Pins[i].Debounce == 0 )
		{
		    pData->Pins[i].Debounce = milliseconds_counter & 0x7F;
		    bOn = 0;
		}
		else 
		{
		    uint8_t ms = (pData->Pins[i].Debounce - milliseconds_counter) & 0x7F;
		    if ( ms > DEBOUNCE_COUNTER )
			pData->Pins[i].Debounce = 0x80;
		    else
			bOn = 0;
		}
	    }
	}

	if ( bOn )
	{
	    if ( !nEncoded )
	    {
		nValue = pData->Pins[i].Bit;		// not binary encoded means only one bit can be on.
		break;
	    }
	    else
		nValue |= pData->Pins[i].Bit;
	}
    }

    pData->LastValue = nValue;
    PackData16( ReportBuffer, nBit, nValue, pData->ReportSize );
}

