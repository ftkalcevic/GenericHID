#ifndef _CONFIGURATIONDEVICE_H_
#define _CONFIGURATIONDEVICE_H_

#include "configuration.h"
#include "stringtable.h"
#include "bytebuffer.h"

class ConfigurationDevice: public Configuration
{
public:
    ConfigurationDevice(void);
    virtual ~ConfigurationDevice(void);
    virtual bool Load( const QDomElement &elem, QString *sError );
    virtual ByteArray GetReportDescriptor(StringTable &table) const;
    virtual ByteArray GetControlConfig( byte /*nReportId*/ ) const { return ByteArray(); }

private:
    ushort m_nVID;
    ushort m_nPID;
    ushort m_nRelease;
    QString m_sManufacturer;
    QString m_sProduct;
    QString m_sSerialNo;
};

#endif
