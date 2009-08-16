#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "stringtable.h"
#include "bytebuffer.h"

class Configuration
{
public:
    Configuration(void);
    virtual ~Configuration(void);

    virtual bool Load( const QDomElement &elem, QString *sError ) = 0;
    virtual ByteArray GetReportDescriptor(StringTable &table) const = 0;		// returns the configuration descriptor (device or config) todo - do we need this? device and config are not handled polymorphically.
    virtual ByteArray GetControlConfig( byte nReportId ) const = 0;				// returns the micro controller application data

    static bool MakePort( const QString &sPort, int &nPort, QString *sError );
    static bool GetPort( const QDomElement &elem, const QString &sPort, int &nPort, QString *sError );
};

#endif
