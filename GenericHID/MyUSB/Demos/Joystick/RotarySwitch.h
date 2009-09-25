#ifndef _ROTARYSWITCH_H_
#define _ROTARYSWITCH_H_



extern void InitRotarySwitch( struct SRotarySwitchControl *pData );
extern void ReadRotarySwitch( struct SRotarySwitchControl *pData, byte **ReportBuffer, byte *nBit );

#endif
