#include "stdafx.h"
#include "hidparser.h"
#include "testinputhatswitch.h"


TestInputHatSwitch::TestInputHatSwitch(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestInput(pHIDItem->ReportID,parent)
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

