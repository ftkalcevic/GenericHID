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
    return pProp;
}

bool ShapePropertyUsage::Configure( QDomElement &node )
{
    m_sDefault = XMLUtility::getAttribute( node, "default", "1:1" );
    return true;
}
