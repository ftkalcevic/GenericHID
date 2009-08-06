#ifndef _SHAPEPROPERTYINT_H_
#define _SHAPEPROPERTYINT_H_


#include "shapeproperty.h"

class ShapePropertyInt :
    public ShapeProperty
{
public:
    ShapePropertyInt(const QString &sName, const QString &sDescription);
    virtual ~ShapePropertyInt(void);
};

#endif
