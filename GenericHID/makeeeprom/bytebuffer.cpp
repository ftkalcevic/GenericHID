#include "stdafx.h"
#include "bytebuffer.h"

ByteBuffer::ByteBuffer(void)
{
}

ByteBuffer::ByteBuffer( const byte *pBuffer, int nLen )
: ByteArray(nLen)
{
    assert( pBuffer != NULL );
    for ( int i = 0; i < nLen; i++, pBuffer++ )
	operator[](i) = *pBuffer;
}

ByteBuffer::ByteBuffer( const ByteArray &buf )
: ByteArray( buf )
{
}

ByteBuffer::~ByteBuffer(void)
{
}

void ByteBuffer::AddByte( byte n ) 
{ 
    this->append( n ); 
}

void ByteBuffer::AddShort( int16_t n ) 
{ 
    AddArray( (byte *)&n, sizeof(n) ) ;
}

void ByteBuffer::AddUShort( uint16_t n ) 
{ 
    AddArray( (byte *)&n, sizeof(n) ) ;
}

void ByteBuffer::AddArray( byte *pBuf, int nLen )
{
    assert( pBuf != NULL );
    if ( pBuf != NULL )
    {
	int nOldSize = this->count();
	this->resize( nOldSize + nLen );
	while ( nLen > 0 )
	{
	    operator[](nOldSize) = *pBuf;
	    pBuf++;
	    nOldSize++;
	    nLen--;
	}
    }
}


void ByteBuffer::AddBuffer( const ByteArray &buf )
{
    *this += buf;
}
