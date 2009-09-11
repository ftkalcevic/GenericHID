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
    byte m_nPort;
    QString m_sName;
    byte m_nBits;
    uint16_t m_nResolution;
};
