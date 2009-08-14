#include "stdafx.h"
#include "makeeeprom.h"

makeeeprom::makeeeprom()
{

}

makeeeprom::~makeeeprom()
{

}

bool makeeeprom::loadFile( const QString &sFile )
{
    QDomDocument doc("GenericHIDDevice");

    QFile file(sFile);
    if (!file.open(QIODevice::ReadOnly))
    {
	m_sLastError = file.errorString();
	return false;
    }

    if (!doc.setContent(&file, false, &m_sLastError)) 
    {
	file.close();
	return false;
    }
    file.close();

    return loadXML( doc );
}

bool makeeeprom::loadXML( const QString &sXML )
{
    QDomDocument doc("GenericHIDDevice");
    if (!doc.setContent(sXML, false, &m_sLastError)) 
	return false;

    return loadXML( doc );
}

bool makeeeprom::loadXML( const QDomDocument &doc )
{
    //m_DeviceConfig = null;
    //m_ConfigConfig = null;
    //m_Controls = new List<Configuration>();
    return true;
}

QByteArray makeeeprom::makeEEPROM()
{
    return QByteArray();
}

