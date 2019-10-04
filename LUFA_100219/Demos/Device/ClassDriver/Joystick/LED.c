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
#include "LED.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything

#define DEBOUNCE_COUNTER	10				// 10 ms

void InitLED( struct SLEDControl *pData )
{
    if ( nSerialDebugLevel > 0 ) UART1_Send_P( PSTR("InitLED\r\n") );

    if ( !bInitialised )
    {
    }

    byte nPort = GET_PORT_ID(pData->Port);
    byte nPin = GET_PORT_PIN(pData->Port);

    // set to off to start with
    byte nOut;
    if ( pData->Options & _BV(LED_SINK) )
	nOut = 1;
    else 
	nOut = 0;
    SetPORTBit( nPort, nPin, nOut );

    // Set DDR - set port to output
    SetDDRBit( nPort, nPin, DDR_OUTPUT );

    bInitialised = true;
}


void WriteLED( struct SLEDControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nOut = ReadPackData16( ReportBuffer, nBit, 1 );

    UART1_Send_P(PSTR("Got LED State "));
    UART1_SendInt( nOut );
    UART1_SendCRLF();

    SetLED( pData, nOut );
}

void SetLED( struct SLEDControl *pData, byte nOut )
{
    if ( pData->Options & _BV(LED_SINK) )
	    nOut = 1 - nOut;

    byte nPort = GET_PORT_ID(pData->Port);
    byte nPin = GET_PORT_PIN(pData->Port);

    SetPORTBit( nPort, nPin, nOut );
}

