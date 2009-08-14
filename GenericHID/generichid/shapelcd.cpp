#include "stdafx.h"
#include "shapelcd.h"

ShapeLCD::ShapeLCD(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapeLCD::~ShapeLCD(void)
{
}

bool ShapeLCD::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    QSet<QString> pinsToCheck, notPinsToCheck;
    notPinsToCheck << "RS" << "R/W" << "E" << "D0" << "D1" << "D2" << "D3" << "D4" << "D5" << "D6" << "D7";
    pinsToCheck << "RS" << "R/W" << "E" << "D4" << "D5" << "D6" << "D7";
    
    if ( GetPropertyValueBool( "8 Bit", values, false ) )
	pinsToCheck << "D0" << "D1" << "D2" << "D3";

    notPinsToCheck.subtract( pinsToCheck );

    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
	bSuccess = false;

    return bSuccess;
}




void ShapeLCD::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakeLCDControl( elem, 
		    GetPropertyValueString("Name",values,""), 
		    GetPropertyValueUsagePage("Usage",values,1),
		    GetPropertyValueUsage("Usage",values,1),
		    GetPropertyValueInt("Rows",values,0),
		    GetPropertyValueInt("Columns",values,0),
		    GetPropertyValueInt("8 Bit",values,0),
		    GetPort(pins, "RS"),
		    GetPort(pins, "R/W"),
		    GetPort(pins, "E"),
		    GetPort(pins, "D0"),
		    GetPort(pins, "D1"),
		    GetPort(pins, "D2"),
		    GetPort(pins, "D3"),
		    GetPort(pins, "D4"),
		    GetPort(pins, "D5"),
		    GetPort(pins, "D6"),
		    GetPort(pins, "D7"),
		    GetPropertyValueInt("Row1Address", values, 0 ),
		    GetPropertyValueInt("Row2Address", values, 64 ),
		    GetPropertyValueInt("Row3Address", values, 20 ),
		    GetPropertyValueInt("Row4Address", values, 84 ) );


}
