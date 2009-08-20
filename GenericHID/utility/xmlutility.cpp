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

static void MakeConversionError( const QDomElement &node, const QString &sAttributeName, const QString &sType, QString *sError )
{
    if ( sError != NULL )
    {
	QString sNode = node.nodeName();
	*sError = QString("Failed to convert attribute '%1' in node <%2> to an %3").arg(sAttributeName,sNode,sType);
    }
}


static void MakeRangeError( const QDomElement &node, const QString &sAttributeName, int n, int min, int max, QString *sError )
{
    if ( sError != NULL )
    {
	QString sNode = node.nodeName();
	*sError = QString("The value for attribute '%1' in node <%2> is out of range.  Got %3, should be from %4 to %5").arg(sAttributeName,sNode).arg(n).arg(min).arg(max);
    }
}

bool XMLUtility::getAttributeString( const QDomElement &node, const QString &sAttributeName, QString &s, QString *sError )
{
    if ( !node.hasAttribute( sAttributeName) )
    {
	QString sNode = node.nodeName();
	if ( sError != NULL )
	    *sError = QString("Failed to find attribute '%1' in node <%2>").arg(sAttributeName,sNode);
	return false;
    }

    s = node.attribute( sAttributeName );
    return true;
}

bool XMLUtility::getAttributeUInt( const QDomElement &node, const QString &sAttributeName, unsigned int &n, unsigned int min, unsigned int max, QString *sError )
{
    QString sData;
    if ( !getAttributeString( node, sAttributeName, sData, sError ) )
	return false;

    bool bOk = false;
    n = sData.toUInt( &bOk, 0 );
    if ( bOk )
    {
	if ( n >= min && n <= max )
	    return true;

	MakeRangeError( node, sAttributeName, n, min, max, sError );
	return false;
    }

    MakeConversionError( node, sAttributeName, "unsigned int", sError );
    return false;
}

bool XMLUtility::getAttributeUShort( const QDomElement &node, const QString &sAttributeName, unsigned short &n, unsigned short min, unsigned short max, QString *sError )
{
    QString sData;
    if ( !getAttributeString( node, sAttributeName, sData, sError ) )
	return false;

    bool bOk = false;
    n = sData.toUShort( &bOk, 0 );
    if ( bOk )
    {
	if ( n >= min && n <= max )
	    return true;

	MakeRangeError( node, sAttributeName, n, min, max, sError );
	return false;
    }

    MakeConversionError( node, sAttributeName, "unsigned short", sError );
    return false;
}

bool XMLUtility::getAttributeByte( const QDomElement &node, const QString &sAttributeName, unsigned char &n, unsigned char min, unsigned char max, QString *sError )
{
    ushort nValue;
    if ( getAttributeUShort( node, sAttributeName, nValue, min, max, sError ) )
    {
	n = nValue;
	return true;
    }
    return false;
}

bool XMLUtility::getAttributeBool( const QDomElement &node, const QString &sAttributeName, bool &b, QString *sError )
{
    QString sData;
    if ( !getAttributeString( node, sAttributeName, sData, sError ) )
	return false;

    if ( sData.compare("true", Qt::CaseInsensitive ) == 0 )
    {
	b = true;
	return true;
    }
    else if ( sData.compare("false", Qt::CaseInsensitive ) == 0 )
    {
	b = false;
	return true;
    }

    bool bOk = false;
    b = sData.toInt( &bOk, 0 ) != 0;
    if ( bOk )
	return true;

    MakeConversionError( node, sAttributeName, "unsigned bool", sError );
    return false;
}

//bool XMLUtility::getAttributeString( QDomElement &pNode, const char *sName, QString &sRet )
//{
//    QString name( sName );
//    bool bHasAttribute = pNode.hasAttribute( name );
//    if ( bHasAttribute )
//    {
//	sRet = pNode.attribute( sName );
//    }
//
//    return bHasAttribute;
//}


// Utility function to get a string attribute
QString XMLUtility::getAttribute( const QDomElement &pNode, const char *sName, const char *sDefault )
{
    QString sRet;
    if ( !getAttributeString( pNode, sName, sRet ) )
	sRet = sDefault;
    return sRet;
}

// Utility function to get a string attribute
QString XMLUtility::getAttribute( const QDomElement &pNode, const char *sName, const QString &sDefault )
{
    QString sRet;
    if ( !getAttributeString( pNode, sName, sRet ) )
	sRet = sDefault;
    return sRet;
}


// Utility function to get an integer attribute
int XMLUtility::getAttribute( const QDomElement &pNode, const char *sName, int nDefault )
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
double XMLUtility::getAttribute( const QDomElement &pNode, const char *sName, double nDefault )
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
bool XMLUtility::getAttribute( const QDomElement &pNode, const char *sName, bool bDefault )
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



QDomElement XMLUtility::firstChildElement(const QDomElement &pNode, const char *sTag)
{
    return pNode.firstChildElement( sTag );
}



QDomNodeList XMLUtility::elementsByTagName(const QDomElement &pNode, const char *sTag)
{
    return pNode.elementsByTagName( sTag );
}



bool XMLUtility::hasAttribute(const QDomElement &pNode, const char *sTag)
{
    return pNode.hasAttribute( sTag );
}


void XMLUtility::setAttribute( QDomElement &pNode, const char *sName, const QString &sValue )
{
    pNode.setAttribute( sName, sValue );
}

void XMLUtility::setAttribute( QDomElement &pNode, const char *sName, const char *sValue )
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

