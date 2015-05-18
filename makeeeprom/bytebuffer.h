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

#ifndef _BYTEBUFFER_H_
#define _BYTEBUFFER_H_

#include "datatypes.h"

typedef QVector<byte> ByteArray;

class ByteBuffer : public ByteArray
{
public:
    ByteBuffer(void);
    ByteBuffer( const byte *pBuffer, int nLen );
    ByteBuffer( const ByteArray & );
    ~ByteBuffer(void);

    void AddByte( byte n );
    void AddShort( int16_t n );
    void AddUShort( uint16_t n );
    void AddArray( byte *pBuf, int nLen );
    void AddBuffer( const ByteArray &buf );
    QString toString() const;
};

#endif
