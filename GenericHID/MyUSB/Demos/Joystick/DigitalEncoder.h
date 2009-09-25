#ifndef _DIGITALENCODER_H_
#define _DIGITALENCODER_H_


extern void InitDigitalEncoder( struct SDigitalEncoderControl *pData );
extern void ReadDigitalEncoder( struct SDigitalEncoderControl *pData, byte **ReportBuffer, byte *nBit );

#endif
