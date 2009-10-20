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

#ifndef MAKEEEPROM_H
#define MAKEEEPROM_H

#include "bytebuffer.h"

class MakeEEPROM
{
public:
    MakeEEPROM();
    ~MakeEEPROM();

    bool loadFile( const QString &sFile );
    bool loadXML( const QString &sXML );
    bool loadXML( const QDomDocument &doc );
    ByteArray makeEEPROM();
    const QString &lastError() const { return m_sLastError; }
    static QString MakeIntelHexFormat( ByteArray &eeprom );

private:
    Logger m_Logger;
    QString m_sLastError;
    class ConfigurationDevice *m_DeviceConfig;
    class ConfigurationConfig *m_ConfigConfig;
    QList<class Control *> m_Controls;
};

#endif // MAKEEEPROM_H
