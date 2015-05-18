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

#ifndef _CONFIGURATIONDEVICE_H_
#define _CONFIGURATIONDEVICE_H_

#include "configuration.h"
#include "stringtable.h"
#include "bytebuffer.h"

class ConfigurationDevice: public Configuration
{
public:
    ConfigurationDevice(void);
    virtual ~ConfigurationDevice(void);
    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetReportDescriptor(StringTable &table) const;
    virtual ByteArray GetControlConfig( byte /*nReportId*/ ) const { return ByteArray(); }

private:
    ushort m_nVID;
    ushort m_nPID;
    ushort m_nRelease;
    QString m_sManufacturer;
    QString m_sProduct;
    QString m_sSerialNo;
};

#endif
