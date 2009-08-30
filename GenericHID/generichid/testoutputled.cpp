#include "stdafx.h"
#include "hidparser.h"
#include "testoutputled.h"

TestOutputLED::TestOutputLED(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestOutput("TestOutputLED",pHIDItem->ReportID,parent)
, m_nValue( pHIDItem->Value )
{
    QHBoxLayout *layout = new QHBoxLayout();
    QCheckBox *chkValue = new QCheckBox(sName);
    layout->addWidget( chkValue );

    connect( chkValue, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)) );

    setLayout( layout );
}

TestOutputLED::~TestOutputLED()
{
}

void TestOutputLED::onToggled(bool bState)
{
    m_nValue = bState ? 1 : 0;
    emit valueChanged(m_nReportId);
}