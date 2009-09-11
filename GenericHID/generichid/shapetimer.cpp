#include "stdafx.h"
#include "shapetimer.h"

ShapeTimer::ShapeTimer(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeTimer::~ShapeTimer(void)
{
}


void ShapeTimer::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &, const QList<PropertyValue *> &values  ) const
{
    MakeCounterControl( elem, 
		      GetPropertyValueString("Name",values,""), 
		      GetPropertyValueUsagePage("Usage",values,1),
		      GetPropertyValueUsage("Usage",values,1),
		      GetPropertyValueInt("Period",values,1),
		      GetPropertyValueInt("Counter Bits",values,1) );

}
