#ifndef _SWITCH_H_
#define _SWITCH_H_


extern void InitSwitch( struct SSwitchControl *pData );
extern void ReadSwitch( struct SSwitchControl *pData, byte **ReportBuffer, byte *nBit );

#endif
