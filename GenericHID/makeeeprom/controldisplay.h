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

#ifndef _CONTROLDISPLAY_H_
#define _CONTROLDISPLAY_H_

#include "control.h"

class ControlDisplay :
    public Control
{
public:
    ControlDisplay(void);
    virtual ~ControlDisplay(void);

    virtual Control::Type type() const { return Control::Display; }
    virtual ByteArray GetHIDReportDescriptor( StringTable & /*table*/, int & /*nBits*/ ) const { assert(false); return ByteArray();}
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, byte &nReportId, byte OutputReportLength[MAX_REPORTS], byte &nMaxOutReportLen ) const = 0;
};

#endif

