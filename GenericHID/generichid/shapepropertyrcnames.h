#ifndef _SHAPEPROPERTYRCNAMES_H_
#define _SHAPEPROPERTYRCNAMES_H_


#include "shapepropertystring.h"

class PropertyValueRCNames : public PropertyValueString
{
public:
    PropertyValueRCNames( const QString &sDefault ) : PropertyValueString(sDefault) {}
    virtual ~PropertyValueRCNames() {}
};



class ShapePropertyRCNames : public ShapePropertyString
{
public:
    ShapePropertyRCNames( const QString &sName, const QString &sDescription );
    virtual ~ShapePropertyRCNames(void);

    virtual PropertyValue *createValue() const { return new PropertyValueRCNames(m_sDefault); }
    virtual QtProperty *getQtProperty();
    virtual void setValue(QtProperty *qtProp, PropertyValue *propVal) const;
    virtual void getValue(QtProperty *qtProp, PropertyValue *propVal) const;

    //QtStringPropertyManager *m_pManager;
};

#endif
