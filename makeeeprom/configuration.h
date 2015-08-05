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

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "stringtable.h"
#include "bytebuffer.h"

class Configuration
{
public:
    Configuration(void);
    virtual ~Configuration(void);

    virtual bool Load( const QDomElement &elem, QString *sError ) = 0;
    virtual ByteArray GetReportDescriptor(StringTable &table) const = 0;		// returns the configuration descriptor (device or config) todo - do we need this? device and config are not handled polymorphically.
    virtual ByteArray GetControlConfig( byte nReportId ) const = 0;				// returns the micro controller application data

    static bool MakePort( const QString &sPort, byte &nPort, QString *sError );
    static bool GetPort( const QDomElement &elem, const QString &sPort, byte &nPort, QString *sError );
};

#endif
