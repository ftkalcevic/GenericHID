#include "Common.h"
#include "Serial.h"
#include "Controls.h"
#include "BicolourLED.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything


void InitBicolourLED( struct SBicolourLEDControl *pData )
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


void WriteBicolourLED( struct SBicolourLEDControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nOut = ReadPackData16( ReportBuffer, nBit, 2 );

    if ( bSerialDebug )
    {
	UART1_Send_P(PSTR("Got BicolourLED State "));
	UART1_SendInt( nOut );
	UART1_SendCRLF();
    }

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

