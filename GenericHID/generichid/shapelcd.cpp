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
#include "shapelcd.h"

ShapeLCD::ShapeLCD(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeLCD::~ShapeLCD(void)
{
}

bool ShapeLCD::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    QSet<QString> pinsToCheck, notPinsToCheck;
    notPinsToCheck << "RS" << "R/W" << "E" << "D0" << "D1" << "D2" << "D3" << "D4" << "D5" << "D6" << "D7";
    pinsToCheck << "RS" << "R/W" << "E" << "D4" << "D5" << "D6" << "D7";
    
    if ( GetPropertyValueBool( "8 Bit", values, false ) )
	pinsToCheck << "D0" << "D1" << "D2" << "D3";

    notPinsToCheck.subtract( pinsToCheck );

    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
	bSuccess = false;

    return bSuccess;
}




void ShapeLCD::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakeLCDControl( elem, 
		    GetPropertyValueString("Name",values,""), 
		    GetPropertyValueUsagePage("Usage",values,1),
		    GetPropertyValueUsage("Usage",values,1),
		    GetPropertyValueInt("Rows",values,0),
		    GetPropertyValueInt("Columns",values,0),
		    GetPropertyValueBool("8 Bit",values,0),
		    GetPort(pins, "RS"),
		    GetPort(pins, "R/W"),
		    GetPort(pins, "E"),
		    GetPort(pins, "D0"),
		    GetPort(pins, "D1"),
		    GetPort(pins, "D2"),
		    GetPort(pins, "D3"),
		    GetPort(pins, "D4"),
		    GetPort(pins, "D5"),
		    GetPort(pins, "D6"),
		    GetPort(pins, "D7"),
		    GetPropertyValueInt("Row1Address", values, 0 ),
		    GetPropertyValueInt("Row2Address", values, 64 ),
		    GetPropertyValueInt("Row3Address", values, 20 ),
		    GetPropertyValueInt("Row4Address", values, 84 ) );


}
