#ifndef _CONTROLLED_H_
#define _CONTROLLED_H_


#include "control.h"

class ControlLED :    public Control
{
public:
    ControlLED(void);
    virtual ~ControlLED(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Output; }

private:
    ushort m_nUsagePage;
    ushort m_nUsage;
    byte m_nPort;
    bool m_bSink;
    QString m_sName;
};

#endif
