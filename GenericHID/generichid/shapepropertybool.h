#ifndef _SHAPEPROPERTYBOOL_H_
#define _SHAPEPROPERTYBOOL_H_


#include "shapeproperty.h"

class ShapePropertyBool :
    public ShapeProperty
{
public:
    ShapePropertyBool( const QString &sName, const QString &sDescription );
    virtual ~ShapePropertyBool(void);
};

#endif
