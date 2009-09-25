#ifndef _TRICOLOURLED_H_
#define _TRICOLOURLED_H_



extern void InitTricolourLED( struct STricolourLEDControl *pData );
extern void WriteTricolourLED( struct STricolourLEDControl *pData, byte **ReportBuffer, byte *nBit );

#endif
