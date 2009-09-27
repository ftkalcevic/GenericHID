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

#ifndef _INTELHEXBUFFER_H_
#define _INTELHEXBUFFER_H_

#include "dfutypes.h"
#include "inttypes.h"

class IntelHexBuffer
{
public:
    IntelHexBuffer();
    ~IntelHexBuffer(void);

    bool isEmpty() { return m_HexBuffer.isEmpty(); }
    MemoryType::MemoryType memoryType() { return m_eMemoryType; }
    QVector<int16_t> &data() { return m_HexBuffer; }
    unsigned int usage() { return m_nUsage; }

    bool load(MemoryType::MemoryType memory, const QString &sBuffer, unsigned int max_size);
    bool loadFile(MemoryType::MemoryType memory, const QString &sHexPath, unsigned int max_size);

private:
    QVector<int16_t> m_HexBuffer;
    unsigned int m_nMaxSize;
    MemoryType::MemoryType m_eMemoryType;
    unsigned int m_nUsage;
};


#endif

