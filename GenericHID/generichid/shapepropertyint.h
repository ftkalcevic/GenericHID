#ifndef _SHAPEPROPERTYINT_H_
#define _SHAPEPROPERTYINT_H_


#include "shapeproperty.h"

class PropertyValueInt : public PropertyValue
{
public:
    PropertyValueInt( int nDefault ) : Value(nDefault) {}
    virtual ~PropertyValueInt() {}

    int Value;
};

class ShapePropertyInt :
    public ShapeProperty
{
public:
    ShapePropertyInt(const QString &sName, const QString &sDescription);
    virtual ~ShapePropertyInt(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
    virtual PropertyValue *createValue() const { return new PropertyValueInt(m_nDefault); }
    virtual void setValue(QtProperty *, PropertyValue *) const;
    virtual void getValue(QtProperty *, PropertyValue *) const;
private:
    int m_nDefault;
    int m_nMin;
    int m_nMax;
};

#endif
