#ifndef _CONTROLLCD_H_
#define _CONTROLLCD_H_


#include "control.h"

class ControlLCD :    public Control
{
public:
    ControlLCD(void);
    virtual ~ControlLCD(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits) const { assert(false); return ByteArray();}
    ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits, int nReportId ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Display; }

private:
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    byte m_nRows;
    byte m_nCols;
    bool m_b8Bit;
    byte m_nPortRS;
    byte m_nPortRW;
    byte m_nPortE;
    byte m_nPortD0;
    byte m_nPortD1;
    byte m_nPortD2;
    byte m_nPortD3;
    byte m_nPortD4;
    byte m_nPortD5;
    byte m_nPortD6;
    byte m_nPortD7;
    byte m_nAddrRow3;
    byte m_nAddrRow2;
    byte m_nAddrRow1;
    byte m_nAddrRow0;
};

#endif
