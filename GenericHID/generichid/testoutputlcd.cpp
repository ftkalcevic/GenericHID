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
#include "testoutputlcd.h"
#include "lcdwidget.h"
#include "usages.h"

TestOutputLCD::TestOutputLCD( HIDDevice *pDevice, HID_CollectionPath_t *pCollection, QWidget *parent )
: TestOutput("TestOutputLCD",0,parent)
, m_pLCD( NULL )
, m_pDevice( pDevice )
, m_pCol( pCollection )
, m_LCDDevice( pDevice, pCollection )
{
    QHBoxLayout *layout2 = new QHBoxLayout();
    m_pLCD = new LCDWidget();

    if ( !m_LCDDevice.Init() )
    {
	LOG_MSG( m_Logger, LogTypes::Error, "Failed to initialise LCD device" );
    }

    m_pLCD->setSize( m_LCDDevice.rows(), m_LCDDevice.columns() );
    QString sName( "LCD" );
    if ( pCollection->StringIndex != 0 )
	sName = pDevice->GetDescriptorString( pCollection->StringIndex );
    QLabel *label = new QLabel(sName);
    QPushButton *button = new QPushButton( "Clear" );
    QLabel * lblSize = new QLabel( QString("%1x%2").arg(m_LCDDevice.rows()).arg(m_LCDDevice.columns()) );
    
    layout2->addWidget( label );
    layout2->addWidget( button );
    layout2->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    layout2->addWidget( lblSize );
    connect( button, SIGNAL(pressed()), this, SLOT(onClearPressed()) );

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout( layout2 );
    layout->addWidget( m_pLCD );

    connect( m_pLCD, SIGNAL(clear()), this, SLOT(onLCDClear()) );
    connect( m_pLCD, SIGNAL(write(int,int,const QString &)), this, SLOT(onLCDWrite(int,int,const QString &)) );

    setLayout( layout );
}

TestOutputLCD::~TestOutputLCD()
{

}


void TestOutputLCD::onClearPressed()
{
    m_pLCD->Clear();
}


void TestOutputLCD::onLCDClear()
{
    QString s( m_LCDDevice.columns(), QChar(' ') );
    for ( int i = 0; i < m_LCDDevice.rows(); i++ )
	m_LCDDevice.LCDWrite(i,0,s);
}

void TestOutputLCD::onLCDWrite(int nRow,int nCol,const QString &s)
{
    m_LCDDevice.LCDWrite(nRow,nCol,s);
}

