#include "stdafx.h"
#include "shapepropertybool.h"

ShapePropertyBool::ShapePropertyBool( const QString &sName, const QString &sDescription )
: ShapeProperty( sName, sDescription )
{
}

ShapePropertyBool::~ShapePropertyBool(void)
{
}


QtProperty *ShapePropertyBool::getQtProperty()
{
    QtProperty *pProp = m_boolManager->addProperty( m_sName );
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

