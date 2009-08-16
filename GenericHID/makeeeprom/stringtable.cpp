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
        nOffset += nDescLen;

        stringBuf.AddByte(nDescLen);
        stringBuf.AddByte(3);     // STRING descriptor type
        foreach (QChar c, s)
            stringBuf.AddShort(c.unicode());
    }

    offsetsBuf.AddBuffer(stringBuf);
    return offsetsBuf;
}
    

