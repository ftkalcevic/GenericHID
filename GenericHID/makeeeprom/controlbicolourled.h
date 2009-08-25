#ifndef _ControlBiColourLED_H_
#define _ControlBiColourLED_H_

#include "control.h"

class ControlBiColourLED : public Control
{
public:
    ControlBiColourLED(void);
    virtual ~ControlBiColourLED(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Output; }

private:
    unsigned short m_nUsagePage;
    unsigned short m_nUsage;
    byte m_nPortA;
    byte m_nPortB;
    QString m_sName;
};

#endif
