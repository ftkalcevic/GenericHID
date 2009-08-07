#include "stdafx.h"
#include "shapeinstancecollection.h"

ShapeInstanceCollection::ShapeInstanceCollection(void)
{
}

ShapeInstanceCollection::~ShapeInstanceCollection(void)
{
}


bool ShapeInstanceCollection::CanAdd( const Shape *pShape, QString &sError )
{
    if ( pShape->maxInstances() > 0 )
    {
	// Make sure we don't exceed the maximum instances of this component (usually just the MCU)
	int count = 0;
	foreach ( ShapeInstance *pInstance, m_Instances )
	{
	    if ( pInstance->shape()->shapeType() == pShape->shapeType() )
		count++;
	}
	if ( count+1 > pShape->maxInstances() )
	{
	    sError = QString("There can only be %1 of this type of component").arg(pShape->maxInstances());
	    return false;
	}
    }
    return true;
}

ShapeInstance *ShapeInstanceCollection::CreateNewShape( const Shape *pShape )
{
    ShapeInstance *pInstance = new ShapeInstance( pShape );
    pInstance->CreateGraphics();
    m_Instances.push_back( pInstance );
    return pInstance;
}
