#include "stdafx.h"
#include "shapemcu.h"

ShapeMCU::ShapeMCU(QDomElement &node, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapeMCU::~ShapeMCU(void)
{
}
