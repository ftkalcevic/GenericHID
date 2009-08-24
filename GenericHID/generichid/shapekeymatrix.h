#ifndef _SHAPEKEYMATRIX_H_
#define _SHAPEKEYMATRIX_H_

#include "shape.h"

class ShapeKeyMatrix :
    public Shape
{
public:
    ShapeKeyMatrix(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeKeyMatrix(void);
    virtual bool Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const;
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;
    virtual void PropertyChanged( QtBrowserItem *item, QList<PropertyValue *> &values ) const;
    virtual void populateProperties( QList<PropertyValue *> &values ) const;
};

#endif
