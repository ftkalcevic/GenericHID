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

#ifndef _CONTROLDIRECTIONALSWITCH_H_
#define _CONTROLDIRECTIONALSWITCH_H_


#include "control.h"

class ControlDirectionalSwitch :    public Control
{
    enum DirectionType
    {
        NS,
        WE,
        _4way,
        _8way
    };
public:
    ControlDirectionalSwitch(void);
    virtual ~ControlDirectionalSwitch(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    DirectionType m_eType;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    bool m_bPullup;
    byte m_nDebounceMs;
    byte m_nPortNW;
    byte m_nPortNE;
    byte m_nPortSW;
    byte m_nPortSE;
    byte m_nPortN;
    byte m_nPortS;
    byte m_nPortW;
    byte m_nPortE;
};

#endif
