#ifndef _SHAPECODEDSELECTORSWITCH_H_
#define _SHAPECODEDSELECTORSWITCH_H_

#include "shape.h"

class ShapeCodedSelectorSwitch :
    public Shape
{
public:
    ShapeCodedSelectorSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeCodedSelectorSwitch(void);
    virtual bool Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const;
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
