#ifndef _CONTROLLCD_H_
#define _CONTROLLCD_H_


#include "control.h"

class ControlLCD :    public Control
{
public:
    ControlLCD(void);
    virtual ~ControlLCD(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Output; }

private:
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    byte m_nRows;
    byte m_nCols;
    bool m_b8Bit;
    int m_nPortRS;
    int m_nPortRW;
    int m_nPortE;
    int m_nPortD0;
    int m_nPortD1;
    int m_nPortD2;
    int m_nPortD3;
    int m_nPortD4;
    int m_nPortD5;
    int m_nPortD6;
    int m_nPortD7;
    byte m_nAddrRow3;
    byte m_nAddrRow2;
    byte m_nAddrRow1;
    byte m_nAddrRow0;
};

#endif
