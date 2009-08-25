#ifndef _CONTROLRGBLED_H_
#define _CONTROLRGBLED_H_

#include "control.h"

class ControlRGBLED : public Control
{
public:
    ControlRGBLED(void);
    virtual ~ControlRGBLED(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Output; }

private:
    unsigned short m_nUsagePage;
    unsigned short m_nUsage;
    byte m_nPortR;
    byte m_nPortG;
    byte m_nPortB;
    bool m_bSink;
    QString m_sName;
};

#endif
