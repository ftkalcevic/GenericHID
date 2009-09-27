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

#ifndef _CONTROLKEYMATRIX_H_
#define _CONTROLKEYMATRIX_H_


#include "control.h"

class ControlKeyMatrix :    public Control
{
public:
    ControlKeyMatrix(void);
    virtual ~ControlKeyMatrix(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    ushort m_nUsagePage;
    ushort m_nUsageMin;
    bool m_bDebounce;
    QList<int> m_Rows;
    QList<int> m_Cols;
    QStringList m_Strings;
};

#endif
