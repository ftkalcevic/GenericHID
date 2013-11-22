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
#include "shapedirectionalswitch.h"

ShapeDirectionalSwitch::ShapeDirectionalSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeDirectionalSwitch::~ShapeDirectionalSwitch(void)
{
}

bool ShapeDirectionalSwitch::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    QSet<QString> pinsToCheck, notPinsToCheck;
    notPinsToCheck << "N" << "S" << "W" << "E" << "NW" << "NE" << "SW" << "SE";

    // We have the Use LEDs property.  Make sure there is nothing on the LED pins, or the Use Status LEDs properties clash
    QString sEnum = GetPropertyValueEnum( "Directions", values, "" );
    if ( sEnum == "2way NS" )
	pinsToCheck << "N" << "S";
    else if ( sEnum == "2way WE" )
	pinsToCheck << "W" << "E";
    else if ( sEnum == "4way" )
	pinsToCheck << "N" << "S" << "W" << "E";
    else if ( sEnum == "8way" )
	pinsToCheck << "N" << "S" << "W" << "E" << "NW" << "NE" << "SW" << "SE";

    notPinsToCheck.subtract( pinsToCheck );

    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
	bSuccess = false;

    return bSuccess;
}


void ShapeDirectionalSwitch::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    QString sDirections = GetPropertyValueEnum("Directions",values,"");

    MakeDirectionalSwitchControl( elem, 
				  GetPropertyValueString("Name",values,""), 
				  GetPropertyValueUsagePage("Usage",values,1),
				  GetPropertyValueUsage("Usage",values,1),
				  GetPropertyValueBool("Pullup",values,true),
				  GetPropertyValueInt("DebounceMs",values, 10),
				  sDirections,
				  GetPort(pins,"N"),
				  GetPort(pins,"S"),
				  GetPort(pins,"E"),
				  GetPort(pins,"W"),
				  GetPort(pins,"NE"),
				  GetPort(pins,"NW"),
				  GetPort(pins,"SE"),
				  GetPort(pins,"SW") );
}
