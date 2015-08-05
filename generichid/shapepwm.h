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

#ifndef _SHAPEPWM_H_
#define _SHAPEPWM_H_

#include "shape.h"

class ShapePWM : public Shape
{
public:
    ShapePWM(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription);
    virtual ~ShapePWM(void);
    virtual void MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const;

    virtual void wireAddedEvent( WireItem *pWire, QList<PropertyValue *> &values ) const;
    void UpdateTimerDetails( ShapeItem *pItem ) const;
protected:
    void UpdateTimerDetails( PinItem *pMCUPin, QList<PropertyValue *> &values ) const;
};

#endif
