#ifndef _SHAPEPOTENTIOMETER_H_
#define _SHAPEPOTENTIOMETER_H_

#include "shape.h"

class ShapePotentiometer : public Shape
{
public:
    ShapePotentiometer(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapePotentiometer(void);
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
