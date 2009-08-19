#ifndef TESTOUTPUTLED_H
#define TESTOUTPUTLED_H

#include "testoutput.h"

class TestOutputLED : public TestOutput
{
    Q_OBJECT

public:
    TestOutputLED(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent=NULL);
    ~TestOutputLED();

public slots:
    void onToggled(bool);

private:
    int &m_nValue;
};

#endif // TESTOUTPUTLED_H
