#include "stdafx.h"
#include "shapepotentiometer.h"

ShapePotentiometer::ShapePotentiometer(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapePotentiometer::~ShapePotentiometer(void)
{
}


void ShapePotentiometer::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakePotentiometerControl( elem, 
			      GetPropertyValueString("Name",values,""), 
			      GetPropertyValueUsagePage("Usage",values,1),
			      GetPropertyValueUsage("Usage",values,1),
			      GetPropertyValueInt("Bits",values,8),
			      GetPort( pins, "Out") );
}
