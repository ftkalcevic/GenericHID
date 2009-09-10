#include "stdafx.h"
#include "shapetricolourled.h"


ShapeTriColourLED::ShapeTriColourLED(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeTriColourLED::~ShapeTriColourLED(void)
{
}


void ShapeTriColourLED::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    QString sType = GetPropertyValueEnum( "Type", values, "Common Cathode" );
    bool bSource = false;
    if ( sType.compare( "Common Anode", Qt::CaseInsensitive ) != 0 )
	bSource = true;

    MakeTricolourLEDControl( elem, 
			    GetPropertyValueString("Name",values,""), 
			    GetPropertyValueUsagePage("Usage",values,1),
			    GetPropertyValueUsage("Usage",values,1),
			    bSource, 
			    GetPort( pins, "1"),
			    GetPort( pins, "2") );
}
