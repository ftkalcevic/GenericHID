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

#ifndef _CONTROLROTARYSWITCH_H_
#define _CONTROLROTARYSWITCH_H_


#include "control.h"

class ControlRotarySwitch :    public Control
{
    struct RotarySwitchPin
    {
        RotarySwitchPin( byte nPort, byte nBit ) : m_nPort(nPort), m_nBit(nBit) {}
        byte m_nPort;
        byte m_nBit;
    };

public:
    ControlRotarySwitch(void);
    virtual ~ControlRotarySwitch(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    QList<RotarySwitchPin> m_Pins;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    bool m_bPullup;
    byte m_nDebounceMs;
    bool m_bEncoded;
    int m_nLogicalMax;
    int m_nReportBits;
    ushort m_nOutputs;
};

#endif
