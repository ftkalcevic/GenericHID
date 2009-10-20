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

#ifndef TESTOUTPUTLED_H
#define TESTOUTPUTLED_H

#include "testoutput.h"

class TestOutputLED : public TestOutput
{
    Q_OBJECT

public:
    TestOutputLED(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent=NULL);
    ~TestOutputLED();

public slots:
    void onToggled(bool);

private:
    int &m_nValue;
};

#endif // TESTOUTPUTLED_H
