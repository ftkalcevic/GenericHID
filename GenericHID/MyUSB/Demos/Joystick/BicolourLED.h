#ifndef _BICOLOURLED_H_
#define _BICOLOURLED_H_


extern void InitBicolourLED( struct SBicolourLEDControl *pData );
extern void WriteBicolourLED( struct SBicolourLEDControl *pData, byte **ReportBuffer, byte *nBit );

#endif
