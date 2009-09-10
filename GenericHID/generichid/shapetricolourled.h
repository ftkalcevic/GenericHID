#ifndef _SHAPETRICOLOURLED_H_
#define _SHAPETRICOLOURLED_H_
#include "shape.h"

class ShapeTriColourLED : public Shape
{
public:
    ShapeTriColourLED(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeTriColourLED(void);
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
