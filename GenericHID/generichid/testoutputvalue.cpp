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