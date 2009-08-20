#ifndef _BYTEBUFFER_H_
#define _BYTEBUFFER_H_

#include "datatypes.h"

typedef QVector<byte> ByteArray;

class ByteBuffer : public ByteArray
{
public:
    ByteBuffer(void);
    ByteBuffer( const byte *pBuffer, int nLen );
    ByteBuffer( const ByteArray & );
    ~ByteBuffer(void);

    void AddByte( byte n );
    void AddShort( int16_t n );
    void AddUShort( uint16_t n );
    void AddArray( byte *pBuf, int nLen );
    void AddBuffer( const ByteArray &buf );
};

#endif
