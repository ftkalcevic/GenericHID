#ifndef _CONFIGURATIONINTERFACE_H_
#define _CONFIGURATIONINTERFACE_H_

#include "configuration.h"


class ConfigurationInterface :public Configuration
{
public:
    ConfigurationInterface(void);
    virtual ~ConfigurationInterface(void);

    virtual bool Load( const QDomElement &elem, QString *sError ) { return true; }
    virtual ByteArray GetReportDescriptor(StringTable &table) const;		
    virtual ByteArray GetControlConfig( byte nReportId ) const { return ByteArray(); }
};

#endif
