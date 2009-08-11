#ifndef _SHAPEPROPERTYBOOL_H_
#define _SHAPEPROPERTYBOOL_H_


#include "shapeproperty.h"

class PropertyValueBool : public PropertyValue
{
public:
    PropertyValueBool( bool bDefault ) : Value(bDefault) {}
    virtual ~PropertyValueBool() {}

    bool Value;
};

class ShapePropertyBool :
    public ShapeProperty
{
public:
    ShapePropertyBool( const QString &sName, const QString &sDescription );
    virtual ~ShapePropertyBool(void);
    virtual QtProperty *getQtProperty();
    bool Configure( QDomElement &node );
    virtual PropertyValue *createValue()  const { return new PropertyValueBool(m_bDefault); }
    virtual void setValue(QtProperty *, PropertyValue *) const;
    virtual void getValue(QtProperty *, PropertyValue *) const;
    virtual QDomElement WriteXML(QDomElement &node, PropertyValue *) const;
    virtual void getXML(QDomElement &node, PropertyValue *) const;

private:
    bool m_bDefault;
};

#endif
