#include "stdafx.h"
#include "testinputvalue.h"
#include "hidparser.h"

TestInputValue::TestInputValue(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestInput(pHIDItem->ReportID,parent)
, m_nValue( pHIDItem->Value )
{
    QHBoxLayout *layout = new QHBoxLayout();
    m_pbValue = new QProgressBar();
    m_pbValue->setRange( pHIDItem->Attributes.LogicalMinimum, pHIDItem->Attributes.LogicalMaximum );
    QLabel *lblName = new QLabel();
    lblName->setText( sName );
    layout->addWidget( m_pbValue );
    layout->addWidget( lblName );

    setLayout( layout );
}

TestInputValue::~TestInputValue()
{
}


void TestInputValue::Update()
{
    m_pbValue->setValue( m_nValue );
}

