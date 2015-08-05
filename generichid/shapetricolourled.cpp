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

#include "stdafx.h"
#include "shapetricolourled.h"


ShapeTriColourLED::ShapeTriColourLED(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeTriColourLED::~ShapeTriColourLED(void)
{
}


void ShapeTriColourLED::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    QString sType = GetPropertyValueEnum( "Type", values, "Common Cathode" );
    bool bSource = false;
    if ( sType.compare( "Common Anode", Qt::CaseInsensitive ) != 0 )
	bSource = true;

    MakeTricolourLEDControl( elem, 
			    GetPropertyValueString("Name",values,""), 
			    GetPropertyValueUsagePage("Usage",values,1),
			    GetPropertyValueUsage("Usage",values,1),
			    bSource, 
			    GetPort( pins, "1"),
			    GetPort( pins, "2") );
}
