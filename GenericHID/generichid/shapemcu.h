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

#ifndef _SHAPEMCU_H_
#define _SHAPEMCU_H_

#include "shape.h"

class ShapeMCU :
    public Shape
{
public:
    ShapeMCU(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapeMCU(void);

    virtual bool Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const;
    virtual void MakeDeviceXML( QDomElement &elem, int nCurrent, const QString &sPowerPin, const QList<PropertyValue *> &values  ) const;
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;

public:
    QString Firmware( const QList<PropertyValue *> &values ) const;
    QString ProgrammerType( const QList<PropertyValue *> &values ) const;
};

#endif
