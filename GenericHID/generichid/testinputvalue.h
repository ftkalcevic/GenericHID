#ifndef TESTINPUTVALUE_H
#define TESTINPUTVALUE_H

#include "testinput.h"

class TestInputValue : public TestInput
{
    Q_OBJECT

public:
    TestInputValue(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent = NULL);
    ~TestInputValue();

    virtual void Update();

private:
    int &m_nValue;
    QProgressBar *m_pbValue;
};

#endif // TESTINPUTVALUE_H
