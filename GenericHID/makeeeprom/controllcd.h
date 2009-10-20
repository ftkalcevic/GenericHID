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

#ifndef _CONTROLLCD_H_
#define _CONTROLLCD_H_


#include "controldisplay.h"

class ControlLCD :    public ControlDisplay
{
public:
    ControlLCD(void);
    virtual ~ControlLCD(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, byte &nReportId, byte OutputReportLength[MAX_REPORTS], byte &nMaxOutReportLen ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;

private:
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    byte m_nRows;
    byte m_nCols;
    bool m_b8Bit;
    byte m_nPortRS;
    byte m_nPortRW;
    byte m_nPortE;
    byte m_nPortD0;
    byte m_nPortD1;
    byte m_nPortD2;
    byte m_nPortD3;
    byte m_nPortD4;
    byte m_nPortD5;
    byte m_nPortD6;
    byte m_nPortD7;
    byte m_nAddrRow3;
    byte m_nAddrRow2;
    byte m_nAddrRow1;
    byte m_nAddrRow0;
};

#endif
