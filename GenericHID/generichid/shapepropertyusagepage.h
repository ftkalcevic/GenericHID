#ifndef _SHAPEPROPERTYUSAGEPAGE_H_
#define _SHAPEPROPERTYUSAGEPAGE_H_

#include "shapeproperty.h"

class ShapePropertyUsagePage :
    public ShapeProperty
{
public:
    ShapePropertyUsagePage(const QString &sName, const QString &sDescription);
    virtual ~ShapePropertyUsagePage(void);
};

#endif
