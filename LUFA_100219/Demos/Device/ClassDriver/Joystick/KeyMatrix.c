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
#include "KeyMatrix.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything


void InitKeyMatrix( struct SKeyMatrixControl *pData )
{
    if ( !bInitialised )
    {
    }

    byte *ptr = pData->Data;
    for ( byte r = 0; r < pData->Rows; r++ )
    {
        byte nPort = GET_PORT_ID(*ptr);
        byte nPin = GET_PORT_PIN(*ptr);

        // Set DDR - set port to input
        SetDDRBit( nPort, nPin, DDR_INPUT );
        SetPORTBit( nPort, nPin, 1 );	// use pullup
        ptr++;
    }

    for ( byte c = 0; c < pData->Cols; c++ )
    {
        byte nPort = GET_PORT_ID(*ptr);
        byte nPin = GET_PORT_PIN(*ptr);

        // Set DDR - set port to output
        SetDDRBit( nPort, nPin, DDR_OUTPUT );
        SetPORTBit( nPort, nPin, 1 );
        ptr++;
    }

    // reset the debounce counters
    if ( pData->DebounceMs )
    {
        DebounceData *dd = (DebounceData *)ptr;
        for ( int i = 0; i < pData->Rows * pData->Cols; i++, dd++ )
        {
            dd->key_down = 0;
            dd->start_time = milliseconds_counter;
        }
    }

    bInitialised = true;
}


void ReadKeyMatrix( struct SKeyMatrixControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte *pRows = pData->Data;
    byte *pCols = pRows + pData->Rows;
    DebounceData *pDebounce = (DebounceData *)(pCols + pData->Cols);

    for ( byte c = 0; c < pData->Cols; c++, pCols++ )
    {
        ClearIOBit( *pCols );			// Pull each output column low, then read the row inputs

        byte *pRow = pRows;
        for ( byte r = 0; r < pData->Rows; r++, pRow++ )
        {
            byte key_down = 1 - GetIOBit( *pRow );

            if ( pData->DebounceMs )
            {
                if ( key_down == pDebounce->key_down )
                {
                    pDebounce->start_time = milliseconds_counter;
                }
                else
                {
                    unsigned char diff = milliseconds_counter - pDebounce->start_time;
                    if ( diff >= pData->DebounceMs )
                    {
                        pDebounce->key_down = key_down;
                        pDebounce->start_time = milliseconds_counter;
                    }
                }
                key_down = pDebounce->key_down;
                pDebounce++;
            }

            PackData16( ReportBuffer, nBit, key_down, 1 );
        }

        SetIOBit( *pCols );		// Restore column output to high
    }
}

