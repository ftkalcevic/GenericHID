#include "stdafx.h"
#include "shapeproperties.h"
#include "shapeitem.h"

ShapeProperties::ShapeProperties(const QString &sShapeName, const QString &sDescription)
: m_topItem( NULL )
, m_sName( sShapeName )
, m_sDescription( sDescription )
{
}

ShapeProperties::~ShapeProperties(void)
{
}

void ShapeProperties::add( ShapeProperty *pProperty )
{
    if ( m_topItem == NULL )
    {
	m_topItem = ShapeProperty::MakeGroupItem( m_sName );
	m_topItem->setToolTip( m_sDescription );
    }

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

int ShapeProperties::find( const QString &sName ) const
{
    for ( int i = 0; i < m_Properties.count(); i++ )
	if ( sName.compare( m_Properties[i]->name(), Qt::CaseInsensitive ) == 0  )
	    return i;

    return -1;
}

QtProperty *ShapeProperties::findQt( const QString &sName ) const
{
    for ( int i = 0; i < m_QtProperties.count(); i++ )
	if ( sName.compare( m_QtProperties[i]->propertyName(), Qt::CaseInsensitive ) == 0  )
	    return m_QtProperties[i];

    return NULL;
}
