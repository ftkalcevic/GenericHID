#ifndef _SHAPEPROPERTYUSAGE_H_
#define _SHAPEPROPERTYUSAGE_H_

#include "shapeproperty.h"

class PropertyValueUsage : public PropertyValue
{
public:
    PropertyValueUsage( const QString &sDefault ) : Value(sDefault) {}
    virtual ~PropertyValueUsage() {}

    QString Value;
};

class ShapePropertyUsage :
    public ShapeProperty
{
public:
    ShapePropertyUsage(const QString &sName, const QString &sDescription);
    virtual ~ShapePropertyUsage(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
    virtual PropertyValue *createValue() const { return new PropertyValueUsage(m_sDefault); }
    virtual void setValue(QtProperty *, PropertyValue *) const;
    virtual void getValue(QtProperty *, PropertyValue *) const;
private:
    QString m_sDefault;
};

#endif
