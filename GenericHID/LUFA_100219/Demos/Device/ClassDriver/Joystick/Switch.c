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
#include "Switch.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything

void InitSwitch( struct SSwitchControl *pData )
{
    if ( nSerialDebugLevel > 0 ) UART1_Send_P( PSTR("InitSwitch\r\n") );

    if ( !bInitialised )
    {
    }

    byte nPort = GET_PORT_ID(pData->Port);
    byte nPin = GET_PORT_PIN(pData->Port);

    // Set DDR - set port to input
    SetDDRBit( nPort, nPin, DDR_INPUT );

    // Set pullup if requested
    if ( pData->Options & _BV(SW_PULLUP) )
	SetPORTBit( nPort, nPin, 1 );

    pData->Debounce.key_down = 0;
    pData->Debounce.start_time = milliseconds_counter;

    bInitialised = true;
}


void ReadSwitch( struct SSwitchControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte key_down = GetIOBit( pData->Port );

    if ( pData->Options & _BV(SW_PULLUP) )	// Pullup inverts button presses.
        key_down = 1 - key_down;

    if ( pData->DebounceMs )
    {
        if ( key_down == pData->Debounce.key_down )
        {
            pData->Debounce.start_time = milliseconds_counter;
        }
        else
        {
            uint8_t diff = milliseconds_counter - pData->Debounce.start_time;
            if ( diff >= pData->DebounceMs )
            {
                pData->Debounce.key_down = key_down;
                pData->Debounce.start_time = milliseconds_counter;
            }
        }
        key_down = pData->Debounce.key_down;
    }

    PackData16( ReportBuffer, nBit, key_down, 1 );
}

