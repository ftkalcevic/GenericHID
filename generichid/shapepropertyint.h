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
    ShapePropertyInt(const QString &sName, const QString &sDescription, bool bEnabled);
    virtual ~ShapePropertyInt(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
    virtual PropertyValue *createValue() const { return new PropertyValueInt(m_nDefault); }
    virtual void setValue(QtProperty *, PropertyValue *) const;
    virtual void getValue(QtProperty *, PropertyValue *) const;
    virtual QDomElement WriteXML(QDomElement &node, PropertyValue *) const;
    virtual void getXML(QDomElement &node, PropertyValue *) const;

private:
    int m_nDefault;
    int m_nMin;
    int m_nMax;
};

#endif
