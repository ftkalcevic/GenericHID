#ifndef _SHAPELCD_H_
#define _SHAPELCD_H_

#include "shape.h"

class ShapeLCD :
    public Shape
{
public:
    ShapeLCD(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeLCD(void);
    virtual bool Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const;
};

#endif
