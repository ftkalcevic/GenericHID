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

#ifndef TESTINPUTVALUE_H
#define TESTINPUTVALUE_H

#include "testinput.h"

class TestInputValue : public TestInput
{
    Q_OBJECT

public:
    TestInputValue(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent = NULL);
    ~TestInputValue();

    virtual void Update();

private:
    int &m_nValue;
    QProgressBar *m_pbValue;
    QLabel *m_lblLogical;
    QLabel *m_lblPhysical;
    int m_nLogicalMin;
    int m_nLogicalMax;
    int m_nPhysicalMin;
    int m_nPhysicalMax;
};

#endif // TESTINPUTVALUE_H
