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
#include "configurationinterface.h"

ConfigurationInterface::ConfigurationInterface(void)
{
}

ConfigurationInterface::~ConfigurationInterface(void)
{
}



ByteArray ConfigurationInterface::GetReportDescriptor(StringTable & /*table*/) const
{
    ByteBuffer InterfaceDescriptor;

    InterfaceDescriptor.AddByte(0);      // Length
    InterfaceDescriptor.AddByte(4);      // INTERFACE Type
    InterfaceDescriptor.AddByte(0);      // Interface number
    InterfaceDescriptor.AddByte(0);      // Alternate Setting
    InterfaceDescriptor.AddByte(2);      // Total endpoints - always 2 = 1 input + 1 output
    InterfaceDescriptor.AddByte(3);      // Class
    InterfaceDescriptor.AddByte(0);      // subclass
    InterfaceDescriptor.AddByte(0);      // protocol
    InterfaceDescriptor.AddByte(0);      // Interface String 
    InterfaceDescriptor[0] = (byte)InterfaceDescriptor.count();

    return InterfaceDescriptor;
}

