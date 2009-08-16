#ifndef _STRINGTABLE_H_
#define _STRINGTABLE_H_

#include "bytebuffer.h"

class StringTable
{
public:
    StringTable(void);
    ~StringTable(void);
    byte operator[] ( const QString &s );
    byte ForceAdd( const QString &s );
    ByteArray GetDescriptor(ushort nOffsetStart) const;
    unsigned int count() const { return m_Strings.count(); }

private:
    QStringList m_Strings;
};

#endif
