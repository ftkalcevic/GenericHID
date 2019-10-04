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

