#include "stdafx.h"
#include "timercounter.h"
#include "deviceconfig.h"

int TimerCounter::m_nClockFrequency = 8000000;


TimerCounter::TimerCounter(void)
{
}

TimerCounter::~TimerCounter(void)
{
}


QString TimerCounter::MakeUserDisplay( const QString sDetails )
{
    int nMode;
    int nPrescaler;
    int nTop;
    ExtractDetails( sDetails, nMode, nPrescaler, nTop );

    double dFreq = (double)m_nClockFrequency / (double)nPrescaler / (double)nTop;
    if ( nMode == TC_MODE_PHASECORRECT )	// phase correct
	dFreq /= 2;
    
    return QString("%1 0-%2").arg(MakeFreq(dFreq)).arg(nTop);
}


void TimerCounter::ExtractDetails( const QString sDetails, int &nMode, int &nPrescaler, int &nTop )
{
    nMode = TC_MODE_FASTPWM;
    nPrescaler = 8;
    nTop = 100;

    QStringList s = sDetails.split(",");

    if ( s.count() > 0 )
	nMode = s[0].toInt();
    if ( s.count() > 1 )
	nPrescaler = s[1].toInt();
    if ( s.count() > 2 )
	nTop = s[2].toInt();
}


QString TimerCounter::MakeFreq( double dFreq )
{
    if ( dFreq >= 1000000 )
	return QString("%1MHz").arg(dFreq/1000000.0, 0, 'f', 1 );
    else if ( dFreq >= 1000 )
	return QString("%1kHz").arg(dFreq/1000.0, 0, 'f', 1 );
    else
	return QString("%1Hz").arg(dFreq, 0, 'f', 1 );
}



double TimerCounter::MakeFreq( int nMode, int nPrescaler, int nTop )
{
    double dFreq = (double)m_nClockFrequency / (double)nPrescaler / (double)nTop;
    if ( nMode == TC_MODE_PHASECORRECT )	// phase correct
	dFreq /= 2;
    return dFreq;
}

