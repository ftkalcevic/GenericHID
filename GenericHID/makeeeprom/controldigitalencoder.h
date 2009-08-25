#ifndef _CONTROLDIGITALENCODER_H_
#define _CONTROLDIGITALENCODER_H_


#include "control.h"

class ControlDigitalEncoder : public Control
{
public:
    ControlDigitalEncoder(void);
    virtual ~ControlDigitalEncoder(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    byte m_nPortA;
    byte m_nPortB;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
};

#endif
