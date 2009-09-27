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
