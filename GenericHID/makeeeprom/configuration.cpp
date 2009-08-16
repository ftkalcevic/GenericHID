#include "stdafx.h"
#include "configuration.h"

Configuration::Configuration(void)
{
}

Configuration::~Configuration(void)
{
}

bool Configuration::MakePort( const QString &sPort, int &nRet, QString *sError )
{
    // Port string is in the format P[A-F][0-7]
    QString s = sPort.trimmed();

    if ( s.length() == 3 && s[0].toUpper() == QChar('P') )
    {
	char port = s[1].toUpper().toAscii();
	int nPin = s[2].digitValue();

	if ( port >= 'A' && port <= 'F' )
	{
	    int nPort = port - 'A';
	    if ( nPin >= 0 && nPin <= 7 )
	    {
		nRet = nPort * 8 + nPin;
		return true;
	    }
	}
    }
    if ( sError != NULL )
	*sError = QString( "Failed to interpret port string '%1'.  It must be in the format P[A-F][0-7], eg PB3").arg(sPort);
    return false;
}


bool Configuration::GetPort( const QDomElement &elem, const QString &sPort, int &nPort, QString *sError )
{
    QString s;
    if ( !XMLUtility::getAttributeString( elem, sPort, s, sError ) )
	return false;
    if ( !MakePort( s, nPort, sError ) )
	return false;
    return true;
}

