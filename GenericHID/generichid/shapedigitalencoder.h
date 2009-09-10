#ifndef _SHAPEDIGITALENCODER_H_
#define _SHAPEDIGITALENCODER_H_

#include "shape.h"

class ShapeDigitalEncoder : public Shape
{
public:
    ShapeDigitalEncoder(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeDigitalEncoder(void);
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
