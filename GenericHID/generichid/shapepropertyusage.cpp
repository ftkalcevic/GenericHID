#include "stdafx.h"
#include "shapepropertyusage.h"

ShapePropertyUsage::ShapePropertyUsage( const QString &sName, const QString &sDescription )
: ShapeProperty( sName, sDescription )
{
}

ShapePropertyUsage::~ShapePropertyUsage(void)
{
}


QtProperty *ShapePropertyUsage::getQtProperty()
{
    QtProperty *pProp = m_usageManager->addProperty( m_sName );
    pProp->setToolTip( m_sDescription );
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
