#ifndef _CONTROLANALOGENCODER_H_
#define _CONTROLANALOGENCODER_H_

#include "control.h"

class ControlAnalogEncoder :public Control
{
public:
    ControlAnalogEncoder(void);
    virtual ~ControlAnalogEncoder(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    int m_nPortA;
    int m_nPortB;
    bool m_bPullupA;
    bool m_bPullupB;
    bool m_bDebounce;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
};

#endif
