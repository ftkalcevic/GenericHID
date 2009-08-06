#ifndef _SHAPEPROPERTYSTRING_H_
#define _SHAPEPROPERTYSTRING_H_


#include "shapeproperty.h"

class ShapePropertyString :
    public ShapeProperty
{
public:
    ShapePropertyString(const QString &sName, const QString &sDescription);
    virtual ~ShapePropertyString(void);
};

#endif
