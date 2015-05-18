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
#include "shapeselectorswitch.h"

const int MAX_SELECTOR_PINS = 12;

ShapeSelectorSwitch::ShapeSelectorSwitch(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
    : Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, sIconFile, nMaxInstances, sDescription)
{
}

ShapeSelectorSwitch::~ShapeSelectorSwitch(void)
{
}

bool ShapeSelectorSwitch::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    int nOutputs = GetPropertyValueInt( "Outputs", values, 0 );

    QSet<QString> pinsToCheck;
    for ( int i = 0; i < nOutputs; i++ )
        pinsToCheck << QString::number( i+1 );

    QSet<QString> notPinsToCheck;
    for ( int i = nOutputs; i < MAX_SELECTOR_PINS; i++ )
        notPinsToCheck << QString::number( i+1 );

    bSuccess = CheckPins( sErrors, pins, pinsToCheck );
    if ( !CheckNotPins( sErrors, pins, notPinsToCheck ) )
        bSuccess = false;

    return bSuccess;
}


void ShapeSelectorSwitch::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    int nBits = GetPropertyValueInt("Outputs",values,0);
    QStringList outputs;
    for ( int i = 0; i < nBits; i++ )
        outputs << GetPort( pins, QString::number( i+1 ) );

    MakeRotarySwitchControl( elem, 
                             GetPropertyValueString("Name",values,""),
                             GetPropertyValueUsagePage("Usage",values,1),
                             GetPropertyValueUsage("Usage",values,1),
                             GetPropertyValueBool("Pullup",values,true),
                             GetPropertyValueInt("DebounceMs",values,10),
                             false,
                             nBits,
                             outputs );
}
