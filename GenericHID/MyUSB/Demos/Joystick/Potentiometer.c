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
	adcData[nActiveBit].nCurrentValue = nSampleSum / ADC_SAMPLES;
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
    if ( pData->Bits != 10 )
	nValue >>= (10 - pData->Bits);

    PackData16( ReportBuffer, nBit, nValue, pData->Bits );
}


