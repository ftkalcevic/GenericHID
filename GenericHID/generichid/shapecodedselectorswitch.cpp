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
#include "shapecodedselectorswitch.h"

ShapeCodedSelectorSwitch::ShapeCodedSelectorSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeCodedSelectorSwitch::~ShapeCodedSelectorSwitch(void)
{
}

bool ShapeCodedSelectorSwitch::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    int nOutputs = GetPropertyValueInt( "Outputs", values, 0 );

    QSet<QString> pinsToCheck, notPinsToCheck;
    notPinsToCheck << "1" << "2" << "4" << "8";

    int nBits = 0;
    nOutputs--;
    while ( nOutputs != 0 )
    {
	nBits++;
	nOutputs >>= 1;
    }

    for ( int i = 0; i < nBits; i++ )
	pinsToCheck << QString::number( 1 << i );

    notPinsToCheck.subtract( pinsToCheck );

    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
	bSuccess = false;

    return bSuccess;
}

void ShapeCodedSelectorSwitch::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    int nValues = GetPropertyValueInt("Outputs",values,0);
    int nBits = 0;
    int nTemp = nValues - 1;
    while ( nTemp != 0 )
    {
	nBits++;
	nTemp >>= 1;
    }
    QStringList outputs;
    for ( int i = 0; i < nBits; i++ )
	outputs << GetPort( pins, QString::number( 1<<i ) );

    MakeRotarySwitchControl( elem, 
			     GetPropertyValueString("Name",values,""), 
			     GetPropertyValueUsagePage("Usage",values,1),
			     GetPropertyValueUsage("Usage",values,1),
			     GetPropertyValueBool("Pullup",values,true),
			     GetPropertyValueBool("Debounce",values,true),
			     true,
			     nValues,
			     outputs );
}
