#include "stdafx.h"
#include "shapedigitalencoder.h"

ShapeDigitalEncoder::ShapeDigitalEncoder(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeDigitalEncoder::~ShapeDigitalEncoder(void)
{
}


void ShapeDigitalEncoder::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakeDigitalEncoderControl( elem, 
			       GetPropertyValueString("Name",values,""), 
			       GetPropertyValueUsagePage("Usage",values,1),
			       GetPropertyValueUsage("Usage",values,1),
			       GetPort( pins, "A"),
			       GetPort( pins, "B") );

}
