#include "stdafx.h"
#include "shapecollection.h"
#include "xmlutility.h"
#include "shape.h"


ShapeCollection::ShapeCollection(void)
{
}

ShapeCollection::~ShapeCollection(void)
{
}


ShapeCollection *ShapeCollection::LoadShapeCollection( const QString &sPath )
{
    QFile file( sPath );
    if ( !file.open(QIODevice::ReadOnly) )
	return NULL;

    QDomDocument doc("Config");
    doc.setContent( &file );
    file.close();

    QDomElement pRootElement = doc.firstChildElement( "Config" );
    if ( pRootElement.isNull() )
    {
	//LOG_MSG( logger, LogTypes::Error, "Root node is not 'Config'" );
	return NULL;
    }

    QDomElement pShapesNode = XMLUtility::firstChildElement( pRootElement, "Shapes" );
    if ( pShapesNode.isNull() )
    {
	//LOG_MSG( logger, LogTypes::Error, "Can't find 'Shapes' node" );
	return NULL;
    }

    ShapeCollection *pCol = new ShapeCollection;
    QDomNodeList pShapes = XMLUtility::elementsByTagName( pShapesNode, "Shape" );
    for ( uint i = 0; i < pShapes.length(); i++ )
    {
	QDomElement pShapeNode = pShapes.item(i).toElement();
	Shape *pShape = Shape::CreateFromXML( pShapeNode );

	if ( pShape != NULL )
	{
	    pCol->m_Shapes.push_back( pShape );
	    pCol->m_ShapeMap.insert( pShape->name(), pShape );
	}
    }

    return pCol;
}


