#ifndef _SHAPEPROPERTYENUM_H_
#define _SHAPEPROPERTYENUM_H_

#include "shapeproperty.h"

class PropertyValueEnum : public PropertyValue
{
public:
    PropertyValueEnum( const QString & sDefault ) : Value(sDefault) {}
    virtual ~PropertyValueEnum() {}

    QString Value;
};


class ShapePropertyEnum :
    public ShapeProperty
{
public:
    ShapePropertyEnum( const QString &sName, const QString &sDescription );
    virtual ~ShapePropertyEnum(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
    virtual PropertyValue *createValue() const { return new PropertyValueEnum(m_sDefault); }
    virtual void setValue(QtProperty *, PropertyValue *) const;
    virtual void getValue(QtProperty *, PropertyValue *) const;
private:
    QStringList m_Enums;
    QString m_sDefault;
};

#endif
