#include "stdafx.h"
#include "shapeled.h"


ShapeLED::ShapeLED(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeLED::~ShapeLED(void)
{
}


void ShapeLED::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakeLEDControl( elem, 
		    GetPropertyValueString("Name",values,""), 
		    GetPropertyValueUsagePage("Usage",values,1),
		    GetPropertyValueUsage("Usage",values,1),
	            GetPort( pins, "In"), 
		    GetPropertyValueBool("Source",values,true) );
}
