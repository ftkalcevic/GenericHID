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
#include "hidparser.h"
#include "testinputhatswitch.h"


TestInputHatSwitch::TestInputHatSwitch(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestInput("TestInputHatSwitch",pHIDItem->ReportID,parent)
, m_nValue( pHIDItem->Value )
{
    m_nMin = pHIDItem->Attributes.LogicalMinimum;
    m_nMax = pHIDItem->Attributes.LogicalMaximum;

    QVBoxLayout *layout = new QVBoxLayout();

    m_pHatswitch = new HatSwitchWidget();
    m_pHatswitch->setMinimumSize( QSize(80,80) );
    QLabel *lblName = new QLabel();
    lblName->setText( sName );
    layout->addWidget( lblName );
    layout->addWidget( m_pHatswitch );

    setLayout( layout );
}

TestInputHatSwitch::~TestInputHatSwitch()
{
}


void TestInputHatSwitch::Update()
{
    // HatswitchWidget expects 0-7
    if ( m_nMin != 0 ) 
	m_nValue -= m_nMin;
    m_pHatswitch->setValue( m_nValue );
}

