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

    QString fromString( enum PinType ePinType )
    {
	QString s;
	for ( int i = 0; i < countof(types); i++ )
	    if ( ePinType & types[i].eType )
	    {
		if ( s.length() > 0 )
		    s += ",";
		s += types[i].sName;
	    }
	return s;
    }
};


Pin::Pin( const QString &sId, PinType::PinType ePinType, const QRect &rect, const QString &sOtherUse )
: m_sId( sId )
, m_ePinType( ePinType )
, m_geometry( rect )
, m_sOtherUse( sOtherUse )
{
}

Pin::~Pin(void)
{
}


Pin *Pin::CreateFromXML( QDomElement node )
{
    QString sId = XMLUtility::getAttribute( node, "id", "" );
    QString sRect = XMLUtility::getAttribute( node, "rect", "" );
    QString sPinType = XMLUtility::getAttribute( node, "type", "" );
    QString sOtherUse = XMLUtility::getAttribute( node, "other", "" );

    PinType::PinType ePinType = PinType::fromString(sPinType);

    QStringList sCoords = sRect.split( QChar(','), QString::SkipEmptyParts );
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    if (sCoords.length() == 4)
    {
        x1 = sCoords[0].toInt();
        y1 = sCoords[1].toInt();
        x2 = sCoords[2].toInt();
        y2 = sCoords[3].toInt();
    }

    return new Pin( sId, ePinType, QRect(QPoint(x1,y1),QPoint(x2,y2)), sOtherUse );
}

