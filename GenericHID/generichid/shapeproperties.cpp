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
    m_Propeties.push_back( pProperty );
}


void ShapeProperties::populate(class ShapeItem *pItem) const
{

}


void ShapeProperties::retreive(class ShapeItem *pItem) const
{
}
