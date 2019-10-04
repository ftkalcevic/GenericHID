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
#include "shapepropertyenum.h"

ShapePropertyEnum::ShapePropertyEnum( const QString &sName, const QString &sDescription, bool bEnabled )
: ShapeProperty( sName, sDescription, bEnabled )
{
}

ShapePropertyEnum::~ShapePropertyEnum(void)
{
}


QtProperty *ShapePropertyEnum::getQtProperty()
{
    QtProperty *pProp = m_enumManager->addProperty(m_sName);
    pProp->setToolTip( m_sDescription );
    pProp->setEnabled( m_bEnabled );
    m_enumManager->setEnumNames( pProp, m_Enums );
    return pProp;
}


bool ShapePropertyEnum::Configure( QDomElement &node )
{
    m_sDefault = XMLUtility::getAttribute( node, "default", "" );

    // Read the enum names
    QDomNodeList enumNodes = XMLUtility::elementsByTagName( node, "enum" );
    for ( uint i = 0; i < enumNodes.length(); i++ )
    {
	QDomElement enumNode = enumNodes.item(i).toElement();
	QString sName = XMLUtility::getAttribute( enumNode, "name", "" );

	if ( sName.length() > 0 && !m_Enums.contains(sName) )
	    m_Enums.push_back( sName );
    }
    return true;
}


void ShapePropertyEnum::setValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueEnum *val = dynamic_cast<PropertyValueEnum *>( propVal );
    if ( val != NULL )
	m_enumManager->setValue( qtProp, m_Enums.indexOf(val->Value) );
}

void ShapePropertyEnum::getValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueEnum *val = dynamic_cast<PropertyValueEnum *>( propVal );
    if ( val != NULL )
	val->Value = m_Enums[m_enumManager->value(qtProp)];
}


QDomElement ShapePropertyEnum::WriteXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueEnum *val = dynamic_cast<PropertyValueEnum *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
    {
	QDomElement propNode = ShapeProperty::WriteXML(node, propVal);
	XMLUtility::setAttribute( propNode, "value", val->Value);
	return propNode;
    }
    return QDomElement();
}


void ShapePropertyEnum::getXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueEnum *val = dynamic_cast<PropertyValueEnum *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
	val->Value = XMLUtility::getAttribute( node, "value", m_sDefault );
}
