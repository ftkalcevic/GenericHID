#include "stdafx.h"
#include "hidparser.h"
#include "testoutputvalue.h"


TestOutputValue::TestOutputValue(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent)
: TestOutput(pHIDItem->ReportID,parent)
, m_nValue( pHIDItem->Value )
{
    QHBoxLayout *layout = new QHBoxLayout();
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange( pHIDItem->Attributes.LogicalMinimum, pHIDItem->Attributes.LogicalMaximum );
    QLabel *name = new QLabel( sName );
    layout->addWidget( slider );
    layout->addWidget( name );

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
}