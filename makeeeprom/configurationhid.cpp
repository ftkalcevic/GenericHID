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
#include "configurationhid.h"

ConfigurationHID::ConfigurationHID(int nDescriptorReportLength)
: m_nDescriptorReportLength(nDescriptorReportLength)
{
}

ConfigurationHID::~ConfigurationHID(void)
{
}


ByteArray ConfigurationHID::GetReportDescriptor(StringTable & /*table*/) const
{
    ByteBuffer HIDDescriptor;

    HIDDescriptor.AddByte(0);	    // Length
    HIDDescriptor.AddByte(0x21);    // HID Type
    HIDDescriptor.AddShort(0x1001); // HID Class specification release
    HIDDescriptor.AddByte(0);	    // Country code
    HIDDescriptor.AddByte(1);	    // Number of descriptors (HID report)
    HIDDescriptor.AddByte(0x22);    // HID report type
    HIDDescriptor.AddShort((uint16_t)m_nDescriptorReportLength);      // HID report length
    HIDDescriptor[0] = (byte)HIDDescriptor.count();

    return HIDDescriptor;
}

