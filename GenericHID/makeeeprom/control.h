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

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stringtable.h"
#include "configuration.h"
#include "hidreportdescriptorbuilder.h"
#include "deviceconfig.h"

class Control : public Configuration
{
public:
    Control(void);
    virtual ~Control(void);
    static Control *MakeControl( const QDomElement &elem, QString *sError );

    enum Type
    {
	Input,
	Output,
	Display
    };

    virtual Type type() const = 0;
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const = 0;	// returns the HID report descriptor data for this control
    virtual ByteArray GetReportDescriptor(StringTable & /*table*/ ) const { return ByteArray(); }
};


#endif
