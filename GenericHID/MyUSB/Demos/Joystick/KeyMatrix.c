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

#define DEBOUNCE_COUNTER	10				// 10 ms

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
    if ( pData->Options & _BV(KM_DEBOUNCE) )
	for ( int i = 0; i < pData->Rows * pData->Cols; i++, ptr++ )
	    *ptr = 0;

    bInitialised = true;
}


void ReadKeyMatrix( struct SKeyMatrixControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nDebounce = pData->Options & _BV(KM_DEBOUNCE);
    byte *pRows = pData->Data;
    byte *pCols = pRows + pData->Rows;
    byte *pDebounce = pCols + pData->Cols;

    for ( byte c = 0; c < pData->Cols; c++, pCols++ )
    {
	ClearIOBit( *pCols );			// Pull each output column low, then read the row inputs

	byte *pRow = pRows;
	for ( byte r = 0; r < pData->Rows; r++, pRow++ )
	{
	    byte bOn = 1 - GetIOBit( *pRow );

	    if ( nDebounce )
	    {
		// if the key has just been pressed, start the debounce counter.  Debounce counter decrements every ms.  Then we take the key.
		if ( !bOn )
		    *pDebounce = 0;
		else
		{
		    if ( *pDebounce & 0x80 )
		    {
			// high bit set - already debounced
		    }
		    else if ( *pDebounce == 0 )
		    {
			*pDebounce = milliseconds_counter & 0x7F;
			bOn = 0;
		    }
		    else 
		    {
			uint8_t ms = (*pDebounce - milliseconds_counter) & 0x7F;
			if ( ms > DEBOUNCE_COUNTER )
			    *pDebounce = 0x80;
			else
			    bOn = 0;
		    }
		}
		pDebounce++;
	    }

	    PackData16( ReportBuffer, nBit, bOn, 1 );
	}

	SetIOBit( *pCols );		// Restore column output to high
    }
}

