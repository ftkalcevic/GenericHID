#ifndef _CONTROLSWITCH_H_
#define _CONTROLSWITCH_H_


#include "control.h"

class ControlSwitch :    public Control
{
public:
    ControlSwitch(void);
    virtual ~ControlSwitch(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    int m_nPort;
    bool m_bPullup;
    bool m_bDebounce;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
};

#endif
