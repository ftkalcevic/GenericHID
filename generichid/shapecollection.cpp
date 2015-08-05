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


