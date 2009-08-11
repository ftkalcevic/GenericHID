#include <QString>
#include "intelhexbuffer.h"
#include "intel_hex.h"
#include "dfucommon.h"



IntelHexBuffer::IntelHexBuffer(MemoryType::MemoryType memory, const QString &sHexPath, unsigned int max_size)
: m_nMaxSize( max_size )
, m_eMemoryType( memory )
{
    m_nUsage = 0;
    m_HexBuffer = intel_hex_to_buffer( sHexPath.toAscii().constData(), max_size, &m_nUsage );
    if( m_HexBuffer.isEmpty() ) 
    {
	ERROR_MSG( "Something went wrong with creating the memory image.\n" );
    }
}


IntelHexBuffer::~IntelHexBuffer(void)
{
}


