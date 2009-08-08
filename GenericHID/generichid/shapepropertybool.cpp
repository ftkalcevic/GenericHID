#include "stdafx.h"
#include "shapepropertybool.h"

ShapePropertyBool::ShapePropertyBool( const QString &sName, const QString &sDescription )
: ShapeProperty( sName, sDescription )
{
}

ShapePropertyBool::~ShapePropertyBool(void)
{
}


QtProperty *ShapePropertyBool::getQtProperty()
{
    QtProperty *pProp = m_boolManager->addProperty( m_sName );
    return pProp;
}


bool ShapePropertyBool::Configure( QDomElement &node )
{
    m_bDefault = XMLUtility::getAttribute( node, "default", false );
    return true;
}
