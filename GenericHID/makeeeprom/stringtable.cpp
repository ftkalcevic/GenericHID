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

#include "stdafx.h"
#include "stringtable.h"

#define English_US		0x0409 
#define English_Australian	0x0c09 

StringTable::StringTable(void)
{
    m_Strings.append( QString(QChar(English_US)) );   // String 0 is always the language id 
}

StringTable::~StringTable(void)
{
}

byte StringTable::operator[] ( const QString &s )
{
    if ( s.isEmpty() )
	return 0;

    int nIndex = m_Strings.indexOf( s );
    if ( nIndex < 0 )
    {
	nIndex = m_Strings.count();
	m_Strings.append( s );
    }

    return (unsigned char)nIndex;
}

byte StringTable::ForceAdd( const QString &s )
{
    int nIndex = m_Strings.count();
    m_Strings.append( s );
    return (byte)nIndex;
}



ByteArray StringTable::GetDescriptor(ushort nOffsetStart) const
{
    ByteBuffer stringBuf;
    ByteBuffer offsetsBuf;

    ushort nOffset = (ushort)(nOffsetStart + m_Strings.count() * 2);
    foreach (QString s, m_Strings)
    {
        byte nDescLen = (byte)(2 + s.length() * 2);

        offsetsBuf.AddShort(nOffset);
        nOffset = (ushort)(nOffset + nDescLen);

        stringBuf.AddByte(nDescLen);
        stringBuf.AddByte(3);     // STRING descriptor type
        foreach (QChar c, s)
            stringBuf.AddShort(c.unicode());
    }

    offsetsBuf.AddBuffer(stringBuf);
    return offsetsBuf;
}
    

