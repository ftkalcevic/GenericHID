#include "Common.h"
#include "Serial.h"
#include "Controls.h"
#include "Switch.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything

#define DEBOUNCE_COUNTER	10				// 10 ms

void InitSwitch( struct SSwitchControl *pData )
{
    if ( bSerialDebug ) UART1_Send_P( PSTR("InitSwitch\r\n") );

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

    pData->Debounce = 0;

    bInitialised = true;
}


void ReadSwitch( struct SSwitchControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte bOn = GetIOBit( pData->Port );

    if ( pData->Options & _BV(SW_PULLUP) )	// Pullup inverts button presses.
	bOn = 1 - bOn;	

    if ( pData->Options & _BV(SW_DEBOUNCE) )
    {
	// if the key has just been pressed, start the debounce counter.  Debounce counter decrements every ms.  Then we take the key.
	if ( !bOn )
	    pData->Debounce = 0;
	else
	{
	    if ( pData->Debounce & 0x80 )
	    {
		// high bit set - already debounced
	    }
	    else if ( pData->Debounce == 0 )
	    {
		pData->Debounce = milliseconds_counter & 0x7F;
		bOn = 0;
	    }
	    else 
	    {
		uint8_t ms = (pData->Debounce - milliseconds_counter) & 0x7F;
		if ( ms > DEBOUNCE_COUNTER )
		    pData->Debounce = 0x80;
		else
		    bOn = 0;
	    }
	}
    }

    PackData16( ReportBuffer, nBit, bOn, 1 );
}
