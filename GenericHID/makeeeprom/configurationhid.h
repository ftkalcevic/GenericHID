#ifndef _CONFIGURATIONHID_H_
#define _CONFIGURATIONHID_H_

#include "configuration.h"

class ConfigurationHID :    public Configuration
{
public:
    ConfigurationHID(int nDescriptorReportLength);
    virtual ~ConfigurationHID(void);

    virtual bool Load( const QDomElement & /*elem*/, QString * /*sError*/ ) { return true; }
    virtual ByteArray GetReportDescriptor(StringTable &table) const;		
    virtual ByteArray GetControlConfig( byte /*nReportId*/ ) const  { return ByteArray(); }
private:
    int m_nDescriptorReportLength;
};

#endif