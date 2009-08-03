#include <QString>
#include "intelhexbuffer.h"
#include "intel_hex.h"

#define DEBUG(...)


IntelHexBuffer::IntelHexBuffer(MemoryType::MemoryType memory, const QString &sHexPath, unsigned int max_size)
: m_eMemoryType( memory )
, m_nMaxSize( max_size )
{
    m_nUsage = 0;
    m_HexBuffer = intel_hex_to_buffer( sHexPath.toAscii().constData(), max_size, &m_nUsage );
    if( m_HexBuffer.isEmpty() ) 
    {
	DEBUG( "Something went wrong with creating the memory image.\n" );
	fprintf( stderr, "Something went wrong with creating the memory image.\n" );
    }
}


IntelHexBuffer::~IntelHexBuffer(void)
{
}


