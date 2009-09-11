#include "dfucommon.h"
#include "intelhexbuffer.h"
#include "intel_hex.h"



IntelHexBuffer::IntelHexBuffer()
: m_nMaxSize( 0 )
, m_eMemoryType( MemoryType::EEPROM )
, m_nUsage(0)
{
}


IntelHexBuffer::~IntelHexBuffer(void)
{
}



bool IntelHexBuffer::load(MemoryType::MemoryType memory, const QString &sBuffer, unsigned int max_size)
{
    m_nMaxSize = max_size;
    m_eMemoryType = memory;
    m_nUsage = 0;
    m_HexBuffer = intel_hex_string_to_buffer( sBuffer, max_size, &m_nUsage );
    if( m_HexBuffer.isEmpty() ) 
	return false;
    else
	return true;
}

bool IntelHexBuffer::loadFile(MemoryType::MemoryType memory, const QString &sHexPath, unsigned int max_size)
{
    m_nMaxSize = max_size;
    m_eMemoryType = memory;
    m_nUsage = 0;
    m_HexBuffer = intel_hex_file_to_buffer( sHexPath.toAscii().constData(), max_size, &m_nUsage );
    if( m_HexBuffer.isEmpty() ) 
	return false;
    else
	return true;
}
