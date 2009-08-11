// hidcomp/hidconfig, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

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

#include "xmlutility.h"
#include "common.h"

#ifndef _WIN32
#define _stricmp strcasecmp
#endif

XMLUtility::XMLUtility(void)
{
}

XMLUtility::~XMLUtility(void)
{
}

// Wrapper function to hide the xerces complexity.
bool XMLUtility::getAttributeString( QDomElement &pNode, const char *sName, QString &sRet )
{
    QString name( sName );
    bool bHasAttribute = pNode.hasAttribute( name );
    if ( bHasAttribute )
    {
	sRet = pNode.attribute( sName );
    }

    return bHasAttribute;
}


// Utility function to get a string attribute
QString XMLUtility::getAttribute( QDomElement &pNode, const char *sName, const char *sDefault )
{
    QString sRet;
    if ( !getAttributeString( pNode, sName, sRet ) )
	sRet = sDefault;
    return sRet;
}

// Utility function to get a string attribute
QString XMLUtility::getAttribute( QDomElement &pNode, const char *sName, const QString &sDefault )
{
    QString sRet;
    if ( !getAttributeString( pNode, sName, sRet ) )
	sRet = sDefault;
    return sRet;
}





// Utility function to get an integer attribute
int XMLUtility::getAttribute( QDomElement &pNode, const char *sName, int nDefault )
{
    int nRet = nDefault;
    QString s;
    if ( getAttributeString( pNode, sName, s ) )
    {
	bool bOk = false;
	nRet = s.toInt( &bOk, 0 );
	if ( !bOk )
	    nRet = nDefault;
    }
    return nRet;
}

// Utility function to get a double attribute
double XMLUtility::getAttribute( QDomElement &pNode, const char *sName, double nDefault )
{
    double nRet = nDefault;
    QString s;
    if ( getAttributeString( pNode, sName, s ) )
    {
	bool bOk = false;
	nRet = s.toDouble( &bOk );
	if ( !bOk )
	    nRet = nDefault;
    }
    return nRet;
}

// Utility function to get a bool attribute.  We accept false/true, 0/not 0
bool XMLUtility::getAttribute( QDomElement &pNode, const char *sName, bool bDefault )
{
    bool bRet = bDefault;
    QString s;
    if ( getAttributeString( pNode, sName, s ) )
    {
	if ( s.compare( "true", Qt::CaseInsensitive ) == 0 )
	    bRet = true;
	else if ( s.compare( "false", Qt::CaseInsensitive ) == 0 )
	    bRet = false;
	else
	{
	    bool bOk = false;
	    bRet = s.toInt(&bOk,0) != 0;
	    if ( !bOk )
		bRet = bDefault;
	}
    }
    return bRet;
}



QDomElement XMLUtility::firstChildElement(QDomElement &pNode, const char *sTag)
{
    return pNode.firstChildElement( sTag );
}



QDomNodeList XMLUtility::elementsByTagName(QDomElement &pNode, const char *sTag)
{
    return pNode.elementsByTagName( sTag );
}



bool XMLUtility::hasAttribute(QDomElement &pNode, const char *sTag)
{
    return pNode.hasAttribute( sTag );
}


void XMLUtility::setAttribute( QDomElement &pNode, const char *sName, const QString sValue )
{
    pNode.setAttribute( sName, sValue );

}

void XMLUtility::setAttribute( QDomElement &pNode, const char *sName, int nValue )
{
    pNode.setAttribute( sName, nValue );
}

void XMLUtility::setAttribute( QDomElement &pNode, const char *sName, double nValue )
{
    pNode.setAttribute( sName, nValue );
}

void XMLUtility::setAttribute( QDomElement &pNode, const char *sName, bool bValue )
{
    if ( bValue )
	pNode.setAttribute( sName, "true" );
    else
	pNode.setAttribute( sName, "false" );
}

