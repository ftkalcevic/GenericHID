#include "stdafx.h"
#include "pin.h"


namespace PinType
{
    struct 
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
	    for ( int i = 0; i < countof(types); i++ )
		if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		{
		    eType = (enum PinType)(eType | types[i].eType);
		    break;
		}
	}
	return eType;
    }
};

namespace PinHAlign
{
    struct 
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
	for ( int i = 0; i < countof(types); i++ )
	    if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		return types[i].eType;
	return Centre;
    }
};

namespace PinVAlign
{
    struct 
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
	for ( int i = 0; i < countof(types); i++ )
	    if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		return types[i].eType;
	return Centre;
    }
};


Pin::Pin( const Shape *pShape, const QString &sId, PinType::PinType ePinType, const QRect &rect, const QString &sOtherUse, bool bEnabled, PinHAlign::PinHAlign eHAlign, PinVAlign::PinVAlign eVAlign, double dRotate )
: m_pShape(pShape)
, m_sId( sId )
, m_ePinType( ePinType )
, m_geometry( rect )
, m_sOtherUse( sOtherUse )
, m_bEnabled(bEnabled)
, m_eHAlign(eHAlign)
, m_eVAlign(eVAlign)
, m_dRotate(dRotate)
{
    m_geometry = m_geometry.normalized();
}

Pin::~Pin(void)
{
}


Pin *Pin::CreateFromXML( QDomElement node, const Shape *pShape )
{
    QString sId = XMLUtility::getAttribute( node, "id", "" );
    QString sRect = XMLUtility::getAttribute( node, "rect", "" );
    QString sPinType = XMLUtility::getAttribute( node, "type", "" );
    PinType::PinType ePinType = PinType::fromString(sPinType);
    QString sOtherUse = XMLUtility::getAttribute( node, "other", "" );
    bool bEnabled = XMLUtility::getAttribute( node, "enabled", true );
    QString sHAlign = XMLUtility::getAttribute( node, "halign", "center" );
    PinHAlign::PinHAlign eHAlign = PinHAlign::fromString(sHAlign);
    QString sVAlign = XMLUtility::getAttribute( node, "valign", "center" );
    PinVAlign::PinVAlign eVAlign = PinVAlign::fromString(sVAlign);
    double dRotate = XMLUtility::getAttribute( node, "rotate", 0.0 );

    QStringList sCoords = sRect.split( QChar(','), QString::SkipEmptyParts );
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    if (sCoords.length() == 4)
    {
        x1 = sCoords[0].toInt();
        y1 = sCoords[1].toInt();
        x2 = sCoords[2].toInt();
        y2 = sCoords[3].toInt();
    }

    return new Pin( pShape, sId, ePinType, QRect(QPoint(x1,y1),QPoint(x2,y2)), sOtherUse, bEnabled, eHAlign, eVAlign, dRotate );
}

