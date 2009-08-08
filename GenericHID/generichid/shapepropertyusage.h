#ifndef _SHAPEPROPERTYUSAGE_H_
#define _SHAPEPROPERTYUSAGE_H_

#include "shapeproperty.h"

class ShapePropertyUsage :
    public ShapeProperty
{
public:
    ShapePropertyUsage(const QString &sName, const QString &sDescription);
    virtual ~ShapePropertyUsage(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
private:
    QString m_sDefault;
};

#endif
