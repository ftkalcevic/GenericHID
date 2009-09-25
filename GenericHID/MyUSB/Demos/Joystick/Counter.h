#ifndef _COUNTER_H_
#define _COUNTER_H_

extern void InitCounter( struct SCounterControl *pData );
extern void ReadCounter( struct SCounterControl *pData, byte **ReportBuffer, byte *nBit );

#endif
