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


QString ByteBuffer::toString() const
{
    QString sOut = "\n";

    QString sLine;
    for (int i = 0; i < count(); i++)
    {
        if (i % 16 == 0)
	{
	    if ( !sLine.isEmpty() )
		sOut += sLine + "\n";
	    sLine = QString("%1  ").arg(i,4,16,QChar('0'));
	}
	sLine += QString("%1 ").arg(at(i),2,16,QChar('0'));
    }
    if ( !sLine.isEmpty() )
	sOut += sLine + "\n";
    return sOut;
}
