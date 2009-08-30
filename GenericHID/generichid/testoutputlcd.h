#ifndef TESTOUTPUTLCD_H
#define TESTOUTPUTLCD_H

#include "testoutput.h"
#include "lcdwidget.h"
#include "hidparser.h"
#include "hiddevice.h"
#include "hidlcddevice.h"

class TestOutputLCD : public TestOutput
{
    Q_OBJECT

public:
    TestOutputLCD( HIDDevice *pDevice, HID_CollectionPath_t *, QWidget *parent=NULL );
    ~TestOutputLCD();

private slots:
    void onClearPressed();
    void onLCDClear();
    void onLCDWrite(int nRow,int nCol,const QString &s);

private:
    void getLCDAttributes( int &nRows, int &nCols );
    void LCDWrite( int nRow, int nCol, QString sText );

    LCDWidget *m_pLCD;
    HIDDevice *m_pDevice;
    HID_CollectionPath_t *m_pCol;
    HIDLCDDevice m_LCDDevice;
};

#endif // TESTOUTPUTLCD_H
