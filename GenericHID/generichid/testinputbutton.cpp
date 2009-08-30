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

