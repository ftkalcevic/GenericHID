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
#include "pin.h"


namespace PinType
{
    struct PinTypesDetals
    {
	enum PinType eType;
	const char *sName;
    } types[] = 
    {
	{ ADC, "ADC" },
	{ Interrupt, "Int" },
	{ IO, "IO" },
	{ PWM, "PWM" },
    };

    enum PinType fromString(const QString &sPinType)
    {
	PinType eType = None;
	QStringList sTypes = sPinType.split( QChar(','), QString::SkipEmptyParts );
	
	foreach (QString s, sTypes) 
	{
	    for ( unsigned int i = 0; i < countof(types); i++ )
		if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		{
		    eType = (enum PinType)(eType | types[i].eType);
		    break;
		}
	}
	return eType;
    }

    QString toString(enum PinType eType)
    {
	QString sRet;
	for ( unsigned int i = 0; i < countof(types); i++ )
	    if ( (eType & types[i].eType) != 0 )
	    {
		if ( !sRet.isEmpty() )
		    sRet += ",";
		sRet += types[i].sName;
	    }
	return sRet;
    }
};

namespace PinHAlign
{
    struct PinHAlignDetails
    {
	enum PinHAlign eType;
	const char *sName;
    } types[] = 
    {
	{ Centre, "Centre" },
	{ Centre, "Center" },
	{ Left, "left" },
	{ Right, "Right" },
	{ OutsideLeft, "outsideLeft" },
	{ OutsideRight, "outsideRight" },
    };

    enum PinHAlign fromString(const QString &s)
    {
	for ( unsigned int i = 0; i < countof(types); i++ )
	    if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		return types[i].eType;
	return Centre;
    }
};

namespace PinVAlign
{
    struct PinVAlignDetails
    {
	enum PinVAlign eType;
	const char *sName;
    } types[] = 
    {
	{ Centre, "Centre" },
	{ Centre, "Center" },
	{ Top, "top" },
	{ Bottom, "bottom" },
	{ Above, "above" },
	{ Below, "below" },
    };

    enum PinVAlign fromString(const QString &s)
    {
	for ( unsigned int i = 0; i < countof(types); i++ )
	    if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		return types[i].eType;
	return Centre;
    }
};


Pin::Pin( const Shape *pShape, const QString &sId, PinType::PinType ePinType, const QRect &rect, const QString &sOtherUse, bool bEnabled, PinHAlign::PinHAlign eHAlign, PinVAlign::PinVAlign eVAlign, double dRotate, bool bShared )
: m_pShape(pShape)
, m_sId( sId )
, m_ePinType( ePinType )
, m_geometry( rect )
, m_sOtherUse( sOtherUse )
, m_bEnabled(bEnabled)
, m_eHAlign(eHAlign)
, m_eVAlign(eVAlign)
, m_dRotate(dRotate)
, m_bShared(bShared)
{
    m_geometry = m_geometry.normalized();
}

Pin::~Pin(void)
{
}


Pin *Pin::CreateFromXML( QDomElement node, const Shape *pShape, QString &sError )
{
    QString sId, sRect, sPinType, sOtherUse, sHAlign, sVAlign;
    PinType::PinType ePinType;
    bool bEnabled, bShared;
    PinHAlign::PinHAlign eHAlign;
    PinVAlign::PinVAlign eVAlign;
    double dRotate;

    if ( !XMLUtility::getAttributeString( node, "id", sId, &sError ) )
	return NULL;
    if ( !XMLUtility::getAttributeString( node, "rect", sRect, &sError ) )
	return NULL;
    if ( !XMLUtility::getAttributeString( node, "type", sPinType, &sError ) )
	return NULL;
    ePinType = PinType::fromString(sPinType);
    sOtherUse = XMLUtility::getAttribute( node, "other", "" );
    bEnabled = XMLUtility::getAttribute( node, "enabled", true );
    sHAlign = XMLUtility::getAttribute( node, "halign", "center" );
    eHAlign = PinHAlign::fromString(sHAlign);
    sVAlign = XMLUtility::getAttribute( node, "valign", "center" );
    eVAlign = PinVAlign::fromString(sVAlign);
    dRotate = XMLUtility::getAttribute( node, "rotate", 0.0 );
    bShared = XMLUtility::getAttribute( node, "shared", false);

    QStringList sCoords = sRect.split( QChar(','), QString::SkipEmptyParts );
    int x1 = 0, y1 = 0, width = 0, height = 0;
    if (sCoords.length() == 4)
    {
        x1 = sCoords[0].toInt();
        y1 = sCoords[1].toInt();
        width = sCoords[2].toInt();
        height = sCoords[3].toInt();
    } 
    else
    {
	sError = QString("Unable to convert '%1' to rectangle on line %2 of config file").arg(sRect).arg(node.lineNumber());
	return NULL;
    }

    return new Pin( pShape, sId, ePinType, QRect(QPoint(x1,y1),QSize(width,height)), sOtherUse, bEnabled, eHAlign, eVAlign, dRotate, bShared );
}

