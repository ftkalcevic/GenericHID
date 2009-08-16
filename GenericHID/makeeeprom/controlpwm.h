#pragma once
#include "control.h"

class ControlPWM : public Control
{
public:
    ControlPWM(void);
    virtual ~ControlPWM(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Output; }

private:
    unsigned short m_nUsagePage;
    unsigned short m_nUsage;
    int m_nPort;
    QString m_sName;
    uint32_t m_nPeriod;
    byte m_nResolution;
};
