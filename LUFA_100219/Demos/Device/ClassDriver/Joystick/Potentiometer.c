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
#include "Potentiometer.h"
#include "Serial.h"

#include <string.h>


#define MAX_ADCS		8			// allocate enough room for all 8 ADCs
#define ADC_SAMPLES		4			// we take multiple samples of the adc and average.  Should be a power of 2.

struct ADCData
{
    bool bInUse;
    uint16_t nCurrentValue;
    uint16_t nRange;
};

static struct ADCData adcData[MAX_ADCS];	// work data for all the adc ports we may be using
static bool bInitialised = false;			// first time through we need to initialise everything
static uint16_t nSampleSum;					// we take multiple samples of the adc and average them
static byte nSample;

void InitPotentiometer( struct SPotentiometerControl *pData )
{
    if ( !bInitialised )
    {
	    memset( adcData, 0, sizeof(adcData) );
    }

    // Make sure the port is correct
    if ( GET_PORT_ID(pData->Port) != PortF )
    {
	    UART1_Send_P( PSTR("Potentiometer found on wrong port - Port=0x"));
	    UART1_SendHex( pData->Port );
	    UART1_SendCRLF();
	    return;
    }

    // Mark the adc active
    int nBit = GET_PORT_PIN(pData->Port);

    adcData[nBit].bInUse = true;

    // pre-scale min/max values (remove the multiple from the loop)
    pData->RangeMin *= ADC_SAMPLES;
    pData->RangeMax *= ADC_SAMPLES;
    adcData[nBit].nRange = pData->RangeMax - pData->RangeMin + 1;

    if ( nSerialDebugLevel > 0 )
    {
	    UART1_Send_P( PSTR("Potentiometer "));
	    UART1_SendHex( nBit );
	    UART1_SendChar( ' ' );
	    UART1_SendInt( pData->RangeMin );
	    UART1_SendChar( ' ' );
	    UART1_SendInt( pData->RangeMax );
	    UART1_SendChar( ' ' );
	    UART1_SendInt( adcData[nBit].nRange );
	    UART1_SendChar( ' ' );
	    UART1_SendInt( pData->Options );
	    UART1_SendChar( ' ' );
	    UART1_SendInt( pData->Bits  );
	    UART1_SendCRLF();
    }
	
    // first time through, start up the adc - we do it via interrupts.
    if ( !bInitialised )
    {
	    // set the adc active for the current value.
	    nSample = 0;
	    nSampleSum = 0;

	    // We want to be able to sample each port about 20 times per second.
	    // So with averaging x4, and all ports x8, this is up to 640 samples per second.  
	    // Average conversion will be about (25+13+13+13)/4 = 16 ADC cycles.  Therefore we
	    // need, at 8MHz, we need at least 10kHz clock. No stress.  Use 100kHz-ish.  
	    ADMUX = _BV(REFS0) | nBit;					// AVcc is reference.  Right adjust result. Use the first pin we were given.
	    ADCSRA = _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);	// Auto trigger. Enable interrupt on completion. prescale /64 =>125kHz
	    ADCSRB = 0;							// Free running.

	    ADCSRA |= _BV(ADEN) | _BV(ADSC);	// Go.
    }
    bInitialised = true;
}


SIGNAL(ADC_vect)
{
    nSampleSum += ADC;
    nSample++;
    if ( nSample == ADC_SAMPLES )
    {
        // Start sampling the next pot.
        byte nActiveBit = ADMUX & 0x7;

        byte nNextBit = (nActiveBit + 1)  & 0x7;
        while ( !adcData[nNextBit].bInUse )
            nNextBit = (nNextBit + 1) & 0x7;

        ADCSRA &= ~(_BV(ADEN) | _BV(ADSC));
        ADMUX = _BV(REFS0) | nNextBit;
        ADCSRA |= _BV(ADEN) | _BV(ADSC);

        // store the new value
        adcData[nActiveBit].nCurrentValue = nSampleSum;
        nSample = 0;
        nSampleSum = 0;
    }
}


void ReadPotentiometer( struct SPotentiometerControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nPin = GET_PORT_PIN(pData->Port);
    cli();
    uint16_t nValue = adcData[nPin].nCurrentValue;
    sei();

    if ( pData->RangeMin != 0 || pData->RangeMax != 0 )
    {
        if ( nValue < pData->RangeMin )
            nValue = pData->RangeMin;
        else if ( nValue > pData->RangeMax )
            nValue = pData->RangeMax;
        uint32_t n = nValue - pData->RangeMin;
        n *= (ADC_SAMPLES <<10); 
        n /= adcData[nPin].nRange;
        nValue = n;
    }   
    nValue /= ADC_SAMPLES;

    if ( pData->Options & (1<<POT_INVERT) )
        nValue = ((1<<10) - 1) - nValue;

    if ( pData->Bits != 10 )
        nValue >>= (10 - pData->Bits);

    PackData16( ReportBuffer, nBit, nValue, pData->Bits );
}

