#include "stdafx.h"
#include "shapepwm.h"


ShapePWM::ShapePWM(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapePWM::~ShapePWM(void)
{
}


void ShapePWM::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakePWMControl( elem, 
		    GetPropertyValueString("Name",values,""), 
		    GetPropertyValueUsagePage("Usage",values,1),
		    GetPropertyValueUsage("Usage",values,1),
		    GetPropertyValueInt("Period",values,1),
		    GetPropertyValueInt("Resolution",values,1),
		    GetPort( pins, "PWM") );
}
