#ifndef _SHAPEPROPERTYTIMER_H_
#define _SHAPEPROPERTYTIMER_H_


#include "shapepropertystring.h"

class PropertyValueTimer : public PropertyValueString
{
public:
    PropertyValueTimer( const QString &sDefault ) : PropertyValueString(sDefault) {}
    virtual ~PropertyValueTimer() {}
};



class ShapePropertyTimer : public ShapePropertyString
{
public:
    ShapePropertyTimer( QtStringPropertyManager *manager, const QString &sName, const QString &sDescription, bool bEnabled );
    virtual ~ShapePropertyTimer(void);

    virtual PropertyValue *createValue() const { return new PropertyValueTimer(m_sDefault); }
    virtual QtProperty *getQtProperty();
    virtual void setValue(QtProperty *qtProp, PropertyValue *propVal) const;
    virtual void getValue(QtProperty *qtProp, PropertyValue *propVal) const;

    QtStringPropertyManager *m_pManager;
};

#endif
