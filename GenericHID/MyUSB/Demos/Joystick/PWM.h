#ifndef _PWM_H_
#define _PWM_H_


extern void InitPWM( struct SPWMControl *pData );
extern void WritePWM( struct SPWMControl *pData, byte **ReportBuffer, byte *nBit );
extern void initialisePWMTimers( struct TimerConfig *pTimers, int nCount );

#endif
