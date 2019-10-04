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
#include "RGB.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything

static void SetPin( byte Port, bool bSink )
{
    byte nPort = GET_PORT_ID(Port);
    byte nPin = GET_PORT_PIN(Port);

    // set to off to start with
    byte nOut;
    if ( bSink )
	nOut = 1;
    else 
	nOut = 0;
    SetPORTBit( nPort, nPin, nOut );

    // Set DDR - set port to output
    SetDDRBit( nPort, nPin, DDR_OUTPUT );

}

void InitRGB( struct SRGBLEDControl *pData )
{
    if ( !bInitialised )
    {
    }
    SetPin( pData->PortR, pData->Options & _BV(RGB_SINK) );
    SetPin( pData->PortG, pData->Options & _BV(RGB_SINK) );
    SetPin( pData->PortB, pData->Options & _BV(RGB_SINK) );

    bInitialised = true;
}


static void SetPortBit( byte Port, byte nOut )
{
    byte nPort = GET_PORT_ID(Port);
    byte nPin = GET_PORT_PIN(Port);
    SetPORTBit( nPort, nPin, nOut );
}

void WriteRGB( struct SRGBLEDControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nOut = ReadPackData16( ReportBuffer, nBit, 3 );

    SetRGB(pData,nOut);
}

void SetRGB( struct SRGBLEDControl *pData, byte nOut )
{
    if ( pData->Options & _BV(RGB_SINK) )
	    nOut = ~nOut;

    SetPortBit( pData->PortR, nOut & RGB_RED );
    SetPortBit( pData->PortG, nOut & RGB_GREEN );
    SetPortBit( pData->PortB, nOut & RGB_BLUE );
}

