#ifndef TESTINPUT_H
#define TESTINPUT_H

#include "testitem.h"

class TestInput : public TestItem
{
    Q_OBJECT

public:
    TestInput(byte nReportId, QWidget *parent);
    virtual ~TestInput();

    virtual void Update() = 0;
    virtual bool isInput() const { return true; }

private:

};

#endif // TESTINPUT_H
