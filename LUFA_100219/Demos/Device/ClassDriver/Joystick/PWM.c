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
#include "PWM.h"
#include "Serial.h"
#include "Joystick.h"

#include <string.h>


static bool bInitialised = false;			// first time through we need to initialise everything

void InitPWM( struct SPWMControl *pData )
{
    if ( !bInitialised )
    {
    }

    // Get the port and pin and set it to output
    byte nPort = GET_PORT_ID(pData->Port);
    byte nPin = GET_PORT_PIN(pData->Port);
    SetDDRBit( nPort, nPin, DDR_OUTPUT );

    // Map the port to the timer registers.  Enable the timer OC?? output.
    // store the index, 0 - 7 back in pData->Port so we can do a quicker 
    // lookup to change;  prescale,top,etc are configured once on start up 
    // from main()

    // We set port on compare match and clear at top so in Fast PWM mode, 0 is 0, and 100, 99%
    // WritePWM()) inverts the value.
    switch ( pData->Port )
    {
	case (PortD*8 + 1): /*1 PD1 OC2B*/ pData->Port = 0; TCCR2A |= _BV(COM2B1) | _BV(COM2B0); OCR2B = pData->Resolution; break;
	case (PortB*8 + 5): /*2 PB5 OC1A*/ pData->Port = 1; TCCR1A |= _BV(COM1A1) | _BV(COM1A0); OCR1A = pData->Resolution; break;
	case (PortB*8 + 6): /*3 PB6 OC1B*/ pData->Port = 2; TCCR1A |= _BV(COM1B1) | _BV(COM1B0); OCR1B = pData->Resolution; break;
	case (PortB*8 + 7): /*4 PB7 OC1C*/ pData->Port = 3; TCCR1A |= _BV(COM1C1) | _BV(COM1C0); OCR1C = pData->Resolution; break;
	case (PortC*8 + 6): /*5 PC6 OC3A*/ pData->Port = 4; TCCR3A |= _BV(COM3A1) | _BV(COM3A0); OCR3A = pData->Resolution; break;
	case (PortC*8 + 5): /*6 PC5 OC3B*/ pData->Port = 5; TCCR3A |= _BV(COM3B1) | _BV(COM3B0); OCR3B = pData->Resolution; break;
	case (PortC*8 + 4): /*7 PC4 OC3C*/ pData->Port = 6; TCCR3A |= _BV(COM3C1) | _BV(COM3C0); OCR3C = pData->Resolution; break;
    }

    bInitialised = true;
}


void WritePWM( struct SPWMControl *pData, byte **ReportBuffer, byte *nBit )
{
    uint16_t nValue = ReadPackData16( ReportBuffer, nBit, pData->Bits ); 
    SetPWM( pData, nValue );
}

void SetPWM( struct SPWMControl *pData, uint16_t nValue )
{
    if ( nValue > pData->Resolution )
        nValue = pData->Resolution;

    if ( pData->Port == 0 )
    {
	    byte n = pData->Resolution - *(byte *)(&nValue);

	    if ( nSerialDebugLevel > 10 )
	    {
	        UART1_Send_P( PSTR("PWM Port=") );
	        UART1_SendHex( pData->Port );
	        UART1_Send_P( PSTR(", Value=") );
	        UART1_SendHex( nValue );
	        UART1_SendCRLF();
	    }

        OCR2B = n;		    /*1 PD1 OC2B*/
    }
    else
    {
	    //if ( nValue >= pData->Resolution - 1)
	    //    nValue = 0xFFFF;
	    nValue = pData->Resolution - nValue;

	    if ( nSerialDebugLevel > 10 )
	    {
	        UART1_Send_P( PSTR("PWM Port=") );
	        UART1_SendHex( pData->Port );
	        UART1_Send_P( PSTR(", Value=") );
	        UART1_SendHex( nValue );
	        UART1_SendCRLF();
	    }

	    switch ( pData->Port )
	    {
	        case 1: /*2 PB5 OC1A*/ OCR1A = nValue; break;
	        case 2: /*3 PB6 OC1B*/ OCR1B = nValue; break;
	        case 3: /*4 PB7 OC1C*/ OCR1C = nValue; break;
	        case 4: /*5 PC6 OC3A*/ OCR3A = nValue; break;
	        case 5: /*6 PC5 OC3B*/ OCR3B = nValue; break;
	        case 6: /*7 PC4 OC3C*/ OCR3C = nValue; break;
	    }
    }
}



void initialisePWMTimers( struct TimerConfig *pTimers, int nCount )
{
    // Program and enable the timers.  The timers don't
    // actually do anything unless a PWM output is defined, although
    // it does use more power.
    for ( int i = 0; i < nCount; i++ )
    {
	struct TimerConfig *pTimer = pTimers + i;
	switch ( i )
	{
	    case 0: // Timer 1
	    case 2: // Timer 3
	    {
		byte nMode;
		byte nPrescaler;

		if ( pTimer->Mode == TC_MODE_FASTPWM )
		    nMode = 14;
		else
		    nMode = 10;

		switch ( pTimer->Prescaler )
		{
		    default:
		    case 0:	nPrescaler = 0;	break;
		    case 1:	nPrescaler = _BV(CS10); break;
		    case 8:	nPrescaler = _BV(CS11); break;
		    case 64:	nPrescaler = _BV(CS11) | _BV(CS10); break;
		    case 256:	nPrescaler = _BV(CS12); break;
		    case 1024:	nPrescaler = _BV(CS12) | _BV(CS10); break;
		}

		if ( i == 0 )
		{
		    TIMSK1 = 0;
		    TCCR1A = nMode & 0x03;
		    TCCR1B = ((nMode & 0x0C) << 1) | nPrescaler;
		    TCCR1C = 0;
		    ICR1 = pTimer->CounterTop;
		}
		else
		{
		    TIMSK3 = 0;
		    TCCR3A = nMode & 0x03;
		    TCCR3B = ((nMode & 0x0C) << 1) | nPrescaler;
		    TCCR3C = 0;
		    ICR3 = pTimer->CounterTop;
		}
		break;
	    }
	    case 1: // Timer 2
	    {
		byte nMode;
		byte nPrescaler;

		if ( pTimer->Mode == TC_MODE_FASTPWM )
		    nMode = 7;
		else
		    nMode = 5;

		switch ( pTimer->Prescaler )
		{
		    default:
		    case 0:	nPrescaler = 0;	break;
		    case 1:	nPrescaler = _BV(CS20); break;
		    case 8:	nPrescaler = _BV(CS21); break;
		    case 32:	nPrescaler = _BV(CS21) | _BV(CS20); break;
		    case 64:	nPrescaler = _BV(CS22); break;
		    case 128:	nPrescaler = _BV(CS22) | _BV(CS20); break;
		    case 256:	nPrescaler = _BV(CS22) | _BV(CS21); break;
		    case 1024:	nPrescaler = _BV(CS22) | _BV(CS21) | _BV(CS20); break;
		}


		TIMSK2 = 0;
		TCCR2A = nMode & 0x03;
		TCCR2B = ((nMode & 0x04) << 1) | nPrescaler;
		OCR2A = *(byte *)&(pTimer->CounterTop);
		break;
	    }
	}
    }
}


