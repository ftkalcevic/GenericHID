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

#include "stdafx.h"
#include "testinputbutton.h"
#include "hidparser.h"

TestInputButton::TestInputButton(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestInput("TestInputButton",pHIDItem->ReportID,parent)
, m_nValue( pHIDItem->Value )
{
    QHBoxLayout *layout = new QHBoxLayout();
    m_lblStatus = new QLabel();
    m_lblStatus->setPixmap( QPixmap(":/GenericHID/redlight") );
    QLabel *lblName = new QLabel();
    lblName->setText( sName );
    layout->addWidget( lblName );
    layout->addWidget( m_lblStatus );

    setLayout( layout );
}

TestInputButton::~TestInputButton()
{

}

void TestInputButton::Update()
{
    bool bState = (m_nValue != 0);
    m_lblStatus->setPixmap( bState ? QPixmap(":/GenericHID/greenlight") : QPixmap(":/GenericHID/redlight") );
}

