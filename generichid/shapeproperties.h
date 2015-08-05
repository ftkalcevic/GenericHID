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

#ifndef _SHAPEPROPERTIES_H_
#define _SHAPEPROPERTIES_H_

#include "shapeproperty.h"

class ShapeProperties
{
public:
    ShapeProperties(const QString &sShapeName, const QString &sDescription);
    ~ShapeProperties(void);

    void add( ShapeProperty *pProperty );
    QtProperty *topItem() const { return m_topItem; }
    void populate(const QList<PropertyValue *> &Values) const;
    void retrieve(const QList<PropertyValue *> &Values) const;
    const QList<ShapeProperty *> &items() const { return m_Properties; }
    int find( const QString &sName ) const;
    QtProperty *findQt( const QString &sName ) const;
    
private:
    QtProperty *m_topItem;
    QString m_sName;
    QString m_sDescription;
    QList<ShapeProperty *> m_Properties;
    QList<QtProperty *> m_QtProperties;
};

#endif
