// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include "configuration.h"

Configuration::Configuration(void)
{
}

Configuration::~Configuration(void)
{
}

bool Configuration::MakePort( const QString &sPort, byte &nRet, QString *sError )
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
		nRet = (byte)(nPort * 8 + nPin);
		return true;
	    }
	}
    }
    if ( sError != NULL )
	*sError = QString( "Failed to interpret port string '%1'.  It must be in the format P[A-F][0-7], eg PB3").arg(sPort);
    return false;
}


bool Configuration::GetPort( const QDomElement &elem, const QString &sPort, byte &nPort, QString *sError )
{
    QString s;
    if ( !XMLUtility::getAttributeString( elem, sPort, s, sError ) )
	return false;
    if ( !MakePort( s, nPort, sError ) )
	return false;
    return true;
}

