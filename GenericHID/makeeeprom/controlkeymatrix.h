#ifndef _CONTROLKEYMATRIX_H_
#define _CONTROLKEYMATRIX_H_


#include "control.h"

class ControlKeyMatrix :    public Control
{
public:
    ControlKeyMatrix(void);
    virtual ~ControlKeyMatrix(void);

    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetHIDReportDescriptor( StringTable &table, int &nBits ) const;
    virtual ByteArray GetControlConfig( byte nReportId ) const;
    virtual Control::Type type() const { return Control::Input; }

private:
    ushort m_nUsagePage;
    ushort m_nUsageMin;
    bool m_bDebounce;
    QList<int> m_Rows;
    QList<int> m_Cols;
    QStringList m_Strings;
};

#endif
