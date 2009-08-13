#ifndef _SHAPEMCU_H_
#define _SHAPEMCU_H_

#include "shape.h"

class ShapeMCU :
    public Shape
{
public:
    ShapeMCU(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeMCU(void);

    virtual bool Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const;
    virtual void MakeDeviceXML( QDomElement &elem, int nCurrent, const QList<PropertyValue *> &values  ) const;
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
};

#endif
