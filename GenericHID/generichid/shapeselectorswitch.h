#ifndef _SHAPESELECTORSWITCH_H_
#define _SHAPESELECTORSWITCH_H_

#include "shape.h"

class ShapeSelectorSwitch :
    public Shape
{
public:
    ShapeSelectorSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeSelectorSwitch(void);
    virtual bool Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const;
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
