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


ShapeCollection *ShapeCollection::LoadShapeCollection( const QString &sPath, QString &sError )
{
    QFile file( sPath );
    if ( !file.open(QIODevice::ReadOnly) )
    {
	sError = QString( "Failed to open config file, '%1': %2").arg(sPath).arg(sError);
	return NULL;
    }

    QDomDocument doc("Config");
    doc.setContent( &file );
    file.close();

    QDomElement pRootElement = doc.firstChildElement( "Config" );
    if ( pRootElement.isNull() )
    {
	sError = QString( "Can't find root node <Config> in config file, '%1'").arg(sPath);
	return NULL;
    }

    QDomElement pShapesNode = XMLUtility::firstChildElement( pRootElement, "Shapes" );
    if ( pShapesNode.isNull() )
    {
	sError = QString( "Can't find root node <Shapes> in config file, '%1'").arg(sPath);
	return NULL;
    }

    ShapeCollection *pCol = new ShapeCollection;
    QDomNodeList pShapes = XMLUtility::elementsByTagName( pShapesNode, "Shape" );
    for ( uint i = 0; i < pShapes.length(); i++ )
    {
	QDomElement pShapeNode = pShapes.item(i).toElement();
	Shape *pShape = Shape::CreateFromXML( pShapeNode, sError );

	if ( pShape != NULL )
	{
	    pCol->m_Shapes.push_back( pShape );
	    pCol->m_ShapeMap.insert( pShape->id(), pShape );
	}
	else
	{
	    delete pCol;
	    return NULL;
	}
    }

    return pCol;
}


