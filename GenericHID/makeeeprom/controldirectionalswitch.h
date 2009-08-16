#ifndef _CONTROLDIRECTIONALSWITCH_H_
#define _CONTROLDIRECTIONALSWITCH_H_


#include "control.h"

class ControlDirectionalSwitch :    public Control
{
    enum DirectionType
    {
	NS,
	WE,
	_4way,
	_8way
    };
public:
    ControlDirectionalSwitch(void);
    virtual ~ControlDirectionalSwitch(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    DirectionType m_eType;
    QString m_sName;
    ushort m_nUsagePage;
    ushort m_nUsage;
    bool m_bPullup;
    bool m_bDebounce;
    int m_nPortNW;
    int m_nPortNE;
    int m_nPortSW;
    int m_nPortSE;
    int m_nPortN;
    int m_nPortS;
    int m_nPortW;
    int m_nPortE;
};

#endif
