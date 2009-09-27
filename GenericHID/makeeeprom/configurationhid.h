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

#ifndef _CONFIGURATIONHID_H_
#define _CONFIGURATIONHID_H_

#include "configuration.h"

class ConfigurationHID :    public Configuration
{
public:
    ConfigurationHID(int nDescriptorReportLength);
    virtual ~ConfigurationHID(void);

    virtual bool Load( const QDomElement & /*elem*/, QString * /*sError*/ ) { return true; }
    virtual ByteArray GetReportDescriptor(StringTable &table) const;		
    virtual ByteArray GetControlConfig( byte /*nReportId*/ ) const  { return ByteArray(); }
private:
    int m_nDescriptorReportLength;
};

#endif

