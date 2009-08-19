#ifndef TESTINPUTHATSWITCH_H
#define TESTINPUTHATSWITCH_H

#include "testinput.h"
#include "hatswitchwidget.h"

class TestInputHatSwitch : public TestInput
{
    Q_OBJECT

public:
    TestInputHatSwitch(const QString &sName, struct HID_ReportItem_t *pHIDItem, QWidget *parent = NULL);
    ~TestInputHatSwitch();

    virtual void Update();

private:
    int &m_nValue;
    HatSwitchWidget *m_pHatswitch;
    int m_nMin, m_nMax;
};

#endif // TESTINPUTHATSWITCH_H
