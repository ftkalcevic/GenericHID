#ifndef _CONTROLROTARYSWITCH_H_
#define _CONTROLROTARYSWITCH_H_


#include "control.h"

class ControlRotarySwitch :    public Control
{
    struct RotarySwitchPin
    {
	RotarySwitchPin( byte nPort, byte nBit ) : m_nPort(nPort), m_nBit(nBit) {}
	byte m_nPort;
        byte m_nBit;
    };

public:
    ControlRotarySwitch(void);
    virtual ~ControlRotarySwitch(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    QList<RotarySwitchPin> m_Pins;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    bool m_bPullup;
    bool m_bDebounce;
    bool m_bEncoded;
    int m_nLogicalMax;
    int m_nReportBits;
};

#endif
