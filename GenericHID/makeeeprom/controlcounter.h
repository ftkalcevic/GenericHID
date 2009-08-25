#ifndef _CONTROLCOUNTER_H_
#define _CONTROLCOUNTER_H_

#include "control.h"

class ControlCounter : public Control
{
public:
    ControlCounter(void);
    virtual ~ControlCounter(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    ushort m_nPeriod;
    byte m_nBits;
};

#endif
