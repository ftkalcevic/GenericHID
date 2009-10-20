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
    ShapePropertyUsage(const QString &sName, const QString &sDescription, bool bEnabled);
    virtual ~ShapePropertyUsage(void);
    virtual QtProperty *getQtProperty();
    virtual bool Configure( QDomElement &node );
    virtual PropertyValue *createValue() const { return new PropertyValueUsage(m_sDefault); }
    virtual void setValue(QtProperty *, PropertyValue *) const;
    virtual void getValue(QtProperty *, PropertyValue *) const;
    virtual QDomElement WriteXML(QDomElement &node, PropertyValue *) const;
    virtual void getXML(QDomElement &node, PropertyValue *) const;

private:
    QString m_sDefault;
};

#endif
