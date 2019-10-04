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
#include "TricolourLED.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything


void InitTricolourLED( struct STricolourLEDControl *pData )
{
    if ( !bInitialised )
    {
    }

    byte nPortA = GET_PORT_ID(pData->PortA);
    byte nPinA = GET_PORT_PIN(pData->PortA);

    byte nPortB = GET_PORT_ID(pData->PortB);
    byte nPinB = GET_PORT_PIN(pData->PortB);

    // set to off to start with
    SetPORTBit( nPortA, nPinA, 0 );
    SetPORTBit( nPortB, nPinB, 0 );

    // Set DDR - set port to output
    SetDDRBit( nPortA, nPinA, DDR_OUTPUT );
    SetDDRBit( nPortB, nPinB, DDR_OUTPUT );

    bInitialised = true;
}


void WriteTricolourLED( struct STricolourLEDControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nOut = ReadPackData16( ReportBuffer, nBit, 2 );

    UART1_Send_P(PSTR("Got TricolourLED State "));
    UART1_SendInt( nOut );
    UART1_SendCRLF();

    SetTricolourLED( pData, nOut );
}

void SetTricolourLED( struct STricolourLEDControl *pData, byte nOut )
{
    byte nA;
    byte nB;
    if ( nOut == 1 )
    {
        nA = 1;
        nB = 0;
    }
    else if ( nOut == 2 )
    {
        nA = 0;
        nB = 1;
    }
    else if ( nOut == 3 )
    {
        nA = 1;
        nB = 1;
    }
    else
    {
        nA = 0;
        nB = 0;
    } 

    byte nPortA = GET_PORT_ID(pData->PortA);
    byte nPinA = GET_PORT_PIN(pData->PortA);

    byte nPortB = GET_PORT_ID(pData->PortB);
    byte nPinB = GET_PORT_PIN(pData->PortB);

    // set to off to start with
    SetPORTBit( nPortA, nPinA, nA );
    SetPORTBit( nPortB, nPinB, nB );
}

