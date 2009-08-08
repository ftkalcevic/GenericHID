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
    //QtProperty *pProp = m_boolManager->addProperty( m_sName );
    //return pProp;
    return NULL;
}

