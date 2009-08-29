#ifndef MAKEEEPROM_H
#define MAKEEEPROM_H

#include "bytebuffer.h"

class MakeEEPROM
{
public:
    MakeEEPROM();
    ~MakeEEPROM();

    bool loadFile( const QString &sFile );
    bool loadXML( const QString &sXML );
    bool loadXML( const QDomDocument &doc );
    ByteArray makeEEPROM();
    const QString &lastError() const { return m_sLastError; }
    static QString MakeIntelHexFormat( ByteArray &eeprom );

private:
    Logger m_Logger;
    QString m_sLastError;
    class ConfigurationDevice *m_DeviceConfig;
    class ConfigurationConfig *m_ConfigConfig;
    QList<class Control *> m_Controls;
};

#endif // MAKEEEPROM_H
