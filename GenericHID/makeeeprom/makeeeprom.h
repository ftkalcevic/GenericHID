#ifndef MAKEEEPROM_H
#define MAKEEEPROM_H

#include "makeeeprom_global.h"

class MAKEEEPROM_EXPORT makeeeprom
{
public:
    makeeeprom();
    ~makeeeprom();

    bool loadFile( const QString &sFile );
    bool loadXML( const QString &sXML );
    bool loadXML( const QDomDocument &doc );
    QByteArray makeEEPROM();

private:
    QString m_sLastError;
};

#endif // MAKEEEPROM_H
