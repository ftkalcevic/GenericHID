#ifndef _DIRSWITCH_H_
#define _DIRSWITCH_H_


extern void InitDirSwitch( struct SDirSwitchControl *pData );
extern void ReadDirSwitch( struct SDirSwitchControl *pData, byte **ReportBuffer, byte *nBit );

#endif
