#include "stdafx.h"
#include "shapeswitch.h"


ShapeSwitch::ShapeSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeSwitch::~ShapeSwitch(void)
{
}


void ShapeSwitch::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakeSwitchControl( elem, 
		       GetPropertyValueString("Name",values,""), 
		       GetPropertyValueUsagePage("Usage",values,1),
		       GetPropertyValueUsage("Usage",values,1),
		       GetPropertyValueBool("Pullup",values,true),
		       GetPropertyValueBool("Debounce",values,true),
		       GetPort( pins, "In") );
}
