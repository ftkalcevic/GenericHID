#pragma once

class TimerCounter
{
public:
    TimerCounter(void);
    ~TimerCounter(void);

    static int m_nClockFrequency;
    static QString MakeUserDisplay( const QString sDetails );
    static void ExtractDetails( const QString sDetails, int &nMode, int &nPrescaler, int &nTop );
    static QString MakeFreq( double dFreq );
};
