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

#ifndef _TESTITEM_H_
#define _TESTITEM_H_

#include <QWidget>

class TestItem : public QWidget
{
    Q_OBJECT

public:
    TestItem(const char *sClass, byte m_nReportId, QWidget *parent = NULL);
    virtual ~TestItem();

    virtual bool isInput() const = 0;
    byte reportId() const { return m_nReportId; }

    static TestItem *CreateItem( class HIDDevice *pDevice, struct HID_ReportItem_t *pItem );

protected:
    byte m_nReportId;
    virtual void paintEvent( QPaintEvent * event );
    Logger m_Logger;
};

#endif // TESTITEM_H
