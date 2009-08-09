#include "stdafx.h"
#include "shapepropertystring.h"

ShapePropertyString::ShapePropertyString( const QString &sName, const QString &sDescription )
: ShapeProperty( sName, sDescription )
{
}

ShapePropertyString::~ShapePropertyString(void)
{
}


QtProperty *ShapePropertyString::getQtProperty()
{
    QtProperty *pProp = m_stringManager->addProperty( m_sName );
    return pProp;
}


bool ShapePropertyString::Configure( QDomElement &node )
{
    m_sDefault = XMLUtility::getAttribute( node, "default", "" );
    return true;
}



void ShapePropertyString::setValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueString *val = dynamic_cast<PropertyValueString *>( propVal );
    if ( val != NULL )
	m_stringManager->setValue( qtProp, val->Value );
}

void ShapePropertyString::getValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueString *val = dynamic_cast<PropertyValueString *>( propVal );
    if ( val != NULL )
	val->Value = m_stringManager->value( qtProp );
}

