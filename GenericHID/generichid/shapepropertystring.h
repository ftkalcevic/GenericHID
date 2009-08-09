#ifndef _SHAPEPROPERTYSTRING_H_
#define _SHAPEPROPERTYSTRING_H_


#include "shapeproperty.h"

class PropertyValueString : public PropertyValue
{
public:
    PropertyValueString( const QString &sDefault ) : Value(sDefault) {}
    virtual ~PropertyValueString() {}

    QString Value;
};

class ShapePropertyString :
    public ShapeProperty
{
public:
    ShapePropertyString(const QString &sName, const QString &sDescription);
    virtual ~ShapePropertyString(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
    virtual PropertyValue *createValue() const { return new PropertyValueString(m_sDefault); }
    virtual void setValue(QtProperty *, PropertyValue *) const;
    virtual void getValue(QtProperty *, PropertyValue *) const;
private:
    QString m_sDefault;
};

#endif
