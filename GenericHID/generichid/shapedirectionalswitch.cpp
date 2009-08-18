#include "stdafx.h"
#include "shapedirectionalswitch.h"

ShapeDirectionalSwitch::ShapeDirectionalSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapeDirectionalSwitch::~ShapeDirectionalSwitch(void)
{
}

bool ShapeDirectionalSwitch::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    QSet<QString> pinsToCheck, notPinsToCheck;
    notPinsToCheck << "N" << "S" << "W" << "E" << "NW" << "NE" << "SW" << "SE";

    // We have the Use LEDs property.  Make sure there is nothing on the LED pins, or the Use Status LEDs properties clash
    QString sEnum = GetPropertyValueEnum( "Directions", values, "" );
    if ( sEnum == "2 N/S" )
	pinsToCheck << "N" << "S";
    else if ( sEnum == "2 W/E" )
	pinsToCheck << "W" << "E";
    else if ( sEnum == "4" )
	pinsToCheck << "N" << "S" << "W" << "E";
    else if ( sEnum == "8" )
	pinsToCheck << "N" << "S" << "W" << "E" << "NW" << "NE" << "SW" << "SE";

    notPinsToCheck.subtract( pinsToCheck );

    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
	bSuccess = false;

    return bSuccess;
}


void ShapeDirectionalSwitch::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    QString sDirections = GetPropertyValueEnum("sDirections",values,"");

    MakeDirectionalSwitchControl( elem, 
				  GetPropertyValueString("Name",values,""), 
				  GetPropertyValueUsagePage("Usage",values,1),
				  GetPropertyValueUsage("Usage",values,1),
				  GetPropertyValueBool("Pullup",values,true),
				  GetPropertyValueBool("Debounce",values,true),
				  sDirections,
				  GetPropertyValueString("N",values,""),
				  GetPropertyValueString("S",values,""),
				  GetPropertyValueString("E",values,""),
				  GetPropertyValueString("W",values,""),
				  GetPropertyValueString("NE",values,""),
				  GetPropertyValueString("NW",values,""),
				  GetPropertyValueString("SE",values,""),
				  GetPropertyValueString("SW",values,"") );
}
