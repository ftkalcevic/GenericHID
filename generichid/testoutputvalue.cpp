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
#include "testoutputvalue.h"


TestOutputValue::TestOutputValue(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestOutput("TestOutputValue",pHIDItem->ReportID,parent)
, m_nValue( pHIDItem->Value )
{
    QHBoxLayout *layout = new QHBoxLayout();
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange( pHIDItem->Attributes.LogicalMinimum, pHIDItem->Attributes.LogicalMaximum );
    slider->setSingleStep( 1 );
    slider->setPageStep( 1 );
    QLabel *name = new QLabel( sName );
    layout->addWidget( name );
    layout->addWidget( slider );
    m_lblValue = new QLabel( "0" );
    layout->addWidget( m_lblValue );

    connect( slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)) );

    setLayout( layout );
}

TestOutputValue::~TestOutputValue()
{
}

void TestOutputValue::onSliderValueChanged(int nNewValue)
{
    m_nValue = nNewValue;
    emit valueChanged(m_nReportId);

    m_lblValue->setText( QString::number(m_nValue) );
}


