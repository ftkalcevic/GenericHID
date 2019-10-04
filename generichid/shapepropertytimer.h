// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
