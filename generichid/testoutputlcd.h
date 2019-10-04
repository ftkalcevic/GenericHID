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

#ifndef TESTOUTPUTLCD_H
#define TESTOUTPUTLCD_H

#include "testoutput.h"
#include "lcdwidget.h"
#include "hidparser.h"
#include "hiddevice.h"
#include "hidlcddevice.h"

class TestOutputLCD : public TestOutput
{
    Q_OBJECT

public:
    TestOutputLCD( HIDDevice *pDevice, HID_CollectionPath_t *, QWidget *parent=NULL );
    ~TestOutputLCD();

private slots:
    void onClearPressed();
    void onCursorToggle(bool);
    void onFunctionSetEdited(const QString &);
    void onLCDClear();
    void onLCDWrite(int nRow,int nCol,const QString &s);
    void onLCDCursor(int nRow, int nCol, bool bCursor, bool bBlink );
    void onLCDFunctionSet(int);

private:
    void getLCDAttributes( int &nRows, int &nCols );
    void LCDWrite( int nRow, int nCol, QString sText );

    LCDWidget *m_pLCD;
    HIDDevice *m_pDevice;
    HID_CollectionPath_t *m_pCol;
    HIDLCDDevice m_LCDDevice;
    QCheckBox *m_chkCursor;
    QCheckBox *m_chkCursorBlink;
};

#endif // TESTOUTPUTLCD_H
