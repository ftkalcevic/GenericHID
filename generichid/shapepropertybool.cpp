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
#include "shapepropertybool.h"

ShapePropertyBool::ShapePropertyBool( const QString &sName, const QString &sDescription, bool bEnabled )
: ShapeProperty( sName, sDescription, bEnabled )
{
}

ShapePropertyBool::~ShapePropertyBool(void)
{
}


QtProperty *ShapePropertyBool::getQtProperty()
{
    QtProperty *pProp = m_boolManager->addProperty( m_sName );
    pProp->setToolTip( m_sDescription );
    pProp->setEnabled( m_bEnabled );
   return pProp;
}


bool ShapePropertyBool::Configure( QDomElement &node )
{
    m_bDefault = XMLUtility::getAttribute( node, "default", false );
    return true;
}

void ShapePropertyBool::setValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueBool *val = dynamic_cast<PropertyValueBool *>( propVal );
    if ( val != NULL )
	m_boolManager->setValue( qtProp, val->Value );
}

void ShapePropertyBool::getValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueBool *val = dynamic_cast<PropertyValueBool *>( propVal );
    if ( val != NULL )
	val->Value = m_boolManager->value( qtProp );
}

QDomElement ShapePropertyBool::WriteXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueBool *val = dynamic_cast<PropertyValueBool *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
    {
	QDomElement propNode = ShapeProperty::WriteXML(node, propVal);
	XMLUtility::setAttribute( propNode, "value", val->Value);
	return propNode;
    }
    return QDomElement();
}


void ShapePropertyBool::getXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueBool *val = dynamic_cast<PropertyValueBool *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
	val->Value = XMLUtility::getAttribute( node, "value", m_bDefault );
}
