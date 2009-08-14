#ifndef _SHAPEPWM_H_
#define _SHAPEPWM_H_

#include "shape.h"

class ShapePWM : public Shape
{
public:
    ShapePWM(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapePWM(void);
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
