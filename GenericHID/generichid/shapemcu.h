#ifndef _SHAPEMCU_H_
#define _SHAPEMCU_H_

#include "shape.h"

class ShapeMCU :
    public Shape
{
public:
    ShapeMCU(QDomElement &node, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeMCU(void);
};

#endif
