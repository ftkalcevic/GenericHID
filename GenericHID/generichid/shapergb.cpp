#include "stdafx.h"
#include "shapergb.h"


ShapeRGB::ShapeRGB(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapeRGB::~ShapeRGB(void)
{
}


void ShapeRGB::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    QString sType = GetPropertyValueEnum( "Type", values, "Common Cathode" );
    bool bSource = false;
    if ( sType.compare( "Common Anode", Qt::CaseInsensitive ) != 0 )
	bSource = true;

    MakeRGBControl( elem, 
		    GetPropertyValueString("Name",values,""), 
		    GetPropertyValueUsagePage("Usage",values,1),
		    GetPropertyValueUsage("Usage",values,1),
		    bSource,
		    GetPort( pins, "R"),
		    GetPort( pins, "G"),
		    GetPort( pins, "B") );
}
