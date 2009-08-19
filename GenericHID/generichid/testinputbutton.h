#ifndef TESTINPUTBUTTON_H
#define TESTINPUTBUTTON_H

#include "testinput.h"

class TestInputButton : public TestInput
{
    Q_OBJECT

public:
    TestInputButton(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent = NULL);
    virtual ~TestInputButton();

    virtual void Update();

private:
    QLabel *m_lblStatus;
    int &m_nValue;
};

#endif // TESTINPUTBUTTON_H
