#ifndef _KEYMATRIX_H_
#define _KEYMATRIX_H_


extern void InitKeyMatrix( struct SKeyMatrixControl *pData );
extern void ReadKeyMatrix( struct SKeyMatrixControl *pData, byte **ReportBuffer, byte *nBit );

#endif
