#ifndef _RGB_H_
#define _RGB_H_


extern void InitRGB( struct SRGBLEDControl *pData );
extern void WriteRGB( struct SRGBLEDControl *pData, byte **ReportBuffer, byte *nBit );

#endif
