#ifndef CONFIGXML_H
#define CONFIGXML_H

#include <QVector>
#include <QDomNode>
#include "datatypes.h"

class ConfigXML
{
public:
    ConfigXML();
    QVector<byte> FindReportDescriptor( unsigned short nVID, unsigned short nPID, byte InterfaceNumber );

private:
    QVector<byte> ReadHexData( QDomNode node );
};

#endif // CONFIGXML_H
