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
#include "shapepropertyusage.h"

ShapePropertyUsage::ShapePropertyUsage( const QString &sName, const QString &sDescription, bool bEnabled )
: ShapeProperty( sName, sDescription, bEnabled )
{
}

ShapePropertyUsage::~ShapePropertyUsage(void)
{
}


QtProperty *ShapePropertyUsage::getQtProperty()
{
    QtProperty *pProp = m_usageManager->addProperty( m_sName );
    pProp->setToolTip( m_sDescription );
    pProp->setEnabled( m_bEnabled );
    return pProp;
}

bool ShapePropertyUsage::Configure( QDomElement &node )
{
    m_sDefault = XMLUtility::getAttribute( node, "default", "1:1" );
    return true;
}

void ShapePropertyUsage::setValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueUsage *val = dynamic_cast<PropertyValueUsage *>( propVal );
    if ( val != NULL )
	m_usageManager->setValue( qtProp, val->Value );
}

void ShapePropertyUsage::getValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueUsage *val = dynamic_cast<PropertyValueUsage *>( propVal );
    if ( val != NULL )
	val->Value = m_usageManager->value( qtProp );
}


QDomElement ShapePropertyUsage::WriteXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueUsage *val = dynamic_cast<PropertyValueUsage *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
    {
	QDomElement propNode = ShapeProperty::WriteXML(node, propVal);
	XMLUtility::setAttribute( propNode, "value", val->Value);
	return propNode;
    }
    return QDomElement();
}

void ShapePropertyUsage::getXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueUsage *val = dynamic_cast<PropertyValueUsage *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
	val->Value = XMLUtility::getAttribute( node, "value", m_sDefault );
}
