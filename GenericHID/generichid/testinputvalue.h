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
    QLabel *m_lblLogical;
    QLabel *m_lblPhysical;
    int m_nLogicalMin;
    int m_nLogicalMax;
    int m_nPhysicalMin;
    int m_nPhysicalMax;
};

#endif // TESTINPUTVALUE_H
