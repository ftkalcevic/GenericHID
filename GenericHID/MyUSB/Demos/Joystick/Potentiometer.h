#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_


extern void InitPotentiometer( struct SPotentiometerControl *pData );
extern void ReadPotentiometer( struct SPotentiometerControl *pData, byte **ReportBuffer, byte *nBit );

#endif
