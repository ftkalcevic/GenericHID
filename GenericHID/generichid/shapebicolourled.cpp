#include "stdafx.h"
#include "shapebicolourled.h"


ShapeBiColourLED::ShapeBiColourLED(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeBiColourLED::~ShapeBiColourLED(void)
{
}


void ShapeBiColourLED::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakeBiColourControl( elem, 
			GetPropertyValueString("Name",values,""), 
			GetPropertyValueUsagePage("Usage",values,1),
			GetPropertyValueUsage("Usage",values,1),
			GetPort( pins, "1"),
			GetPort( pins, "2") );
}
