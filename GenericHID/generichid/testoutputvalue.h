#ifndef TESTOUTPUTVALUE_H
#define TESTOUTPUTVALUE_H

#include "testoutput.h"

class TestOutputValue : public TestOutput
{
    Q_OBJECT

public:
    TestOutputValue(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent=NULL);
    ~TestOutputValue();

public slots:
    void onSliderValueChanged(int);

private:
    int &m_nValue;
};

#endif // TESTOUTPUTVALUE_H
