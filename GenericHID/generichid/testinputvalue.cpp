#include "stdafx.h"
#include "testinputvalue.h"
#include "hidparser.h"

TestInputValue::TestInputValue(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestInput("TestInputValue",pHIDItem->ReportID,parent)
, m_nValue( pHIDItem->Value )
{
    QGridLayout *layout = new QGridLayout();
    m_pbValue = new QProgressBar();
    m_pbValue->setRange( pHIDItem->Attributes.LogicalMinimum, pHIDItem->Attributes.LogicalMaximum );
    m_pbValue->setTextVisible( false );
    m_pbValue->setMinimumWidth( 60 );
    QLabel *lblName = new QLabel();
    lblName->setText( sName );
    layout->addWidget( lblName, 0, 0 );
    layout->addWidget( m_pbValue, 1, 0 );

    layout->addWidget( new QLabel("Logical"), 0, 1 );
    layout->addWidget( new QLabel("Physical"), 1, 1 );

    m_lblLogical = new QLabel();
    m_lblPhysical = new QLabel();
    layout->addWidget( m_lblLogical, 0, 2 );
    layout->addWidget( m_lblPhysical, 1, 2 );

    setLayout( layout );

    m_nLogicalMin = pHIDItem->Attributes.LogicalMinimum;
    m_nLogicalMax = pHIDItem->Attributes.LogicalMaximum;

    m_nPhysicalMin = pHIDItem->Attributes.PhysicalMinimum;
    m_nPhysicalMax = pHIDItem->Attributes.PhysicalMaximum;
}

TestInputValue::~TestInputValue()
{
}


void TestInputValue::Update()
{
    m_pbValue->setValue( m_nValue );
    m_lblLogical->setText( QString("%1").arg(m_nValue) );

    int nPhysical = m_nValue;
    if ( m_nPhysicalMin != m_nPhysicalMax)
        nPhysical = (m_nPhysicalMax - m_nPhysicalMin) * (m_nValue - m_nLogicalMin) / (m_nLogicalMax - m_nLogicalMin) + m_nPhysicalMin;

    m_lblPhysical->setText(QString("%1").arg(nPhysical) );
}

