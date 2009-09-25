#ifndef _LED_H_
#define _LED_H_


extern void InitLED( struct SLEDControl *pData );
extern void WriteLED( struct SLEDControl *pData, byte **ReportBuffer, byte *nBit );

#endif
