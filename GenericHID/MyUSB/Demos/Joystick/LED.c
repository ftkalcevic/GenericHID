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
    if ( bSerialDebug ) UART1_Send_P( PSTR("InitLED\r\n") );

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

    if ( pData->Options & _BV(LED_SINK) )
	nOut = 1 - nOut;

    byte nPort = GET_PORT_ID(pData->Port);
    byte nPin = GET_PORT_PIN(pData->Port);

    SetPORTBit( nPort, nPin, nOut );
}

