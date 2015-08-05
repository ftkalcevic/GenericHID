#include "configxml.h"
#include "common.h"
#include "log.h"
#include "xmlutility.h"

#include <QFile>
#include <QCoreApplication>
#include <QStringList>


ConfigXML::ConfigXML()
{
}

QVector<byte> ConfigXML::ReadHexData( QDomNode node )
{
    QVector<byte> data;

    if ( node.childNodes().count() > 0 )
    {
        QDomNode child = node.firstChild();

        QString sHex = child.nodeValue();
        QStringList sData = sHex.split(" ", QString::SkipEmptyParts);
        foreach ( QString s, sData)
            data << s.toInt(0,16);
    }
    return data;
}

QVector<byte> ConfigXML::FindReportDescriptor( unsigned short nVID, unsigned short nPID, byte nInterface )
{
    QVector<byte> ret;

    Logger logger( QCoreApplication::applicationName(), "ConfigXML" );

    const QString fileName = "hidconfig.xml";

    QFile file( fileName );
    if ( !file.open(QIODevice::ReadOnly) )
        return ret;
    QDomDocument doc("HIDComp");
    doc.setContent( &file );
    file.close();

    QDomElement pRootElement = doc.firstChildElement( "HIDComp" );
    if ( pRootElement.isNull() )
    {
        LOG_MSG( logger, LogTypes::Error, "Root node is not 'HIDComp'" );
        return ret;
    }

    QDomNodeList pItems = XMLUtility::elementsByTagName( pRootElement, "ReportDescriptorOverride" );
    for ( uint i = 0; i < pItems.length(); i++ )
    {
        QDomNode node = pItems.item(i);
        QDomElement elem = node.toElement();

        unsigned short VID = (unsigned short)XMLUtility::getAttribute( elem, "VID", "0" ).toUShort(NULL,16);
        unsigned short PID = (unsigned short)XMLUtility::getAttribute( elem, "PID", "0" ).toUShort(NULL,16);
        int Interface = XMLUtility::getAttribute( elem, "Interface", -1 );

        if ( VID == nVID &&
             PID == nPID &&
             Interface == nInterface )
        {
            ret = ReadHexData( node );
            break;
        }
    }

    return ret;
}

