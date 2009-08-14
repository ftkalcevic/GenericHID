#ifndef _SHAPEBICOLOURLED_H_
#define _SHAPEBICOLOURLED_H_

#include "shape.h"

class ShapeBiColourLED : public Shape
{
public:
    ShapeBiColourLED(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeBiColourLED(void);
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
