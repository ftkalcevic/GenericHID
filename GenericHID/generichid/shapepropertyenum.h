#ifndef _SHAPEPROPERTYENUM_H_
#define _SHAPEPROPERTYENUM_H_

#include "shapeproperty.h"

class ShapePropertyEnum :
    public ShapeProperty
{
public:
    ShapePropertyEnum( const QString &sName, const QString &sDescription );
    virtual ~ShapePropertyEnum(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
private:
    QStringList m_Enums;
    QString m_sDefault;
};

#endif
