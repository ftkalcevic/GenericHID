#ifndef _CONTROLPOTENTIOMETER_H_
#define _CONTROLPOTENTIOMETER_H_


#include "control.h"

class ControlPotentiometer:     public Control
{
public:
    ControlPotentiometer(void);
    ~ControlPotentiometer(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    byte m_nPort;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    byte m_nBits;
};

#endif
