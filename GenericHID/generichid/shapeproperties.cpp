#include "stdafx.h"
#include "shapeproperties.h"
#include "shapeitem.h"

ShapeProperties::ShapeProperties(void)
: m_topItem( NULL )
{
}

ShapeProperties::~ShapeProperties(void)
{
}

void ShapeProperties::add( ShapeProperty *pProperty )
{
    if ( m_topItem == NULL )
	m_topItem = ShapeProperty::MakeGroupItem( "Component" );

    QtProperty *pQtProp = pProperty->getQtProperty();
    if ( pQtProp != NULL )
	m_topItem->addSubProperty( pQtProp );
    m_Properties.push_back( pProperty );
    m_QtProperties.push_back( pQtProp );
}


void ShapeProperties::populate(const QList<PropertyValue *> &Values) const
{
    for ( int i = 0; i < m_Properties.count(); i++ )
	m_Properties[i]->setValue( m_QtProperties[i], Values[i] );
}


void ShapeProperties::retreive(const QList<PropertyValue *> &Values) const
{
    for ( int i = 0; i < m_Properties.count(); i++ )
	m_Properties[i]->getValue( m_QtProperties[i], Values[i] );
}
