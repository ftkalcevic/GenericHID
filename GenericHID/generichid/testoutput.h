#ifndef TESTOUTPUT_H
#define TESTOUTPUT_H

#include "testitem.h"

class TestOutput : public TestItem
{
    Q_OBJECT

public:
    TestOutput(byte nReportId, QWidget *parent = NULL);
    virtual ~TestOutput();
    virtual bool isInput() const { return false; }

signals:
    void valueChanged(int nReportId);

private:

};

#endif // TESTOUTPUT_H
