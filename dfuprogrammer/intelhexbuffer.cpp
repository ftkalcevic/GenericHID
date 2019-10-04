// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

bool IntelHexBuffer::append(MemoryType::MemoryType memory, const QString &sBuffer, unsigned int max_size)
{
    m_nMaxSize = max_size;
    m_eMemoryType = memory;
    m_nUsage = 0;
    intel_hex_string_to_buffer_append( m_HexBuffer, sBuffer, max_size, &m_nUsage );
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


bool IntelHexBuffer::isInUse( int nAddrStart, int nAddrEnd )
{
    for ( int i = nAddrStart; i <= nAddrEnd && i < m_HexBuffer.size(); i++ )
    {
	if ( m_HexBuffer[i] != -1 )
	    return true;
    }
    return false;
}