#include "stdafx.h"
#include "shapepropertyint.h"

ShapePropertyInt::ShapePropertyInt( const QString &sName, const QString &sDescription )
: ShapeProperty( sName, sDescription )
{
}

ShapePropertyInt::~ShapePropertyInt(void)
{
}

QtProperty *ShapePropertyInt::getQtProperty()
{
    QtProperty *pProp = m_intManager->addProperty( m_sName );
    pProp->setToolTip( m_sDescription );
    m_intManager->setMinimum( pProp, m_nMin );
    m_intManager->setMaximum( pProp, m_nMax );
    return pProp;
}

bool ShapePropertyInt::Configure( QDomElement &node )
{
    m_nDefault = XMLUtility::getAttribute( node, "default", 0 );
    m_nMin = XMLUtility::getAttribute( node, "min", 0 );
    m_nMax = XMLUtility::getAttribute( node, "max", 65535 );
    return true;
}



void ShapePropertyInt::setValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueInt *val = dynamic_cast<PropertyValueInt *>( propVal );
    if ( val != NULL )
	m_intManager->setValue( qtProp, val->Value );
}

void ShapePropertyInt::getValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueInt *val = dynamic_cast<PropertyValueInt *>( propVal );
    if ( val != NULL )
	val->Value = m_intManager->value( qtProp );
}


QDomElement ShapePropertyInt::WriteXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueInt *val = dynamic_cast<PropertyValueInt *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
    {
	QDomElement propNode = ShapeProperty::WriteXML(node, propVal);
	XMLUtility::setAttribute( propNode, "value", val->Value);
	return propNode;
    }
    return QDomElement();
}


void ShapePropertyInt::getXML(QDomElement &node, PropertyValue *propVal) const
{
    PropertyValueInt *val = dynamic_cast<PropertyValueInt *>( propVal );
    assert( val != NULL );
    if ( val != NULL )
	val->Value = XMLUtility::getAttribute( node, "value", m_nDefault );
}
