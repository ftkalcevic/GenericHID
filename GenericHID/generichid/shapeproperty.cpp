#include "stdafx.h"
#include "shapeproperty.h"
#include "shapepropertybool.h"
#include "shapepropertyenum.h"
#include "shapepropertyint.h"
#include "shapepropertystring.h"
#include "shapepropertyusage.h"
#include "shapepropertyusagepage.h"

namespace  PropertyType
{
    struct 
    {
	enum PropertyType eType;
	const char *sName;
    } types[] = 
    {
	{ String, "String" },
        { Enum, "Enum" },
        { Bool, "Bool" },
        { Int, "Int" },
        { UsagePage, "UsagePage" },
	{ Usage, "Usage" },
    };

    enum PropertyType fromString( const QString &s )
    {
	for ( int i = 0; i < countof(types); i++ )
	    if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		return types[i].eType;
	return None;
    }


};

ShapeProperty::ShapeProperty( const QString &sName, const QString &sDescription )
: m_sName(sName)
, m_sDescription( sDescription )
{
}

ShapeProperty::~ShapeProperty(void)
{
}


ShapeProperty *ShapeProperty::CreateShapeProperty( QDomElement &node, const QString &sName, const QString &sDescription, PropertyType::PropertyType ePropType )
{
    ShapeProperty *pProp = NULL;

    switch ( ePropType )
    {
	case PropertyType::String:	pProp = new ShapePropertyString( sName, sDescription ); break;
        case PropertyType::Enum:	pProp = new ShapePropertyEnum( sName, sDescription ); break;
        case PropertyType::Bool:	pProp = new ShapePropertyBool( sName, sDescription ); break;
        case PropertyType::Int:		pProp = new ShapePropertyInt( sName, sDescription ); break;
        case PropertyType::UsagePage:	pProp = new ShapePropertyUsagePage( sName, sDescription ); break;
        case PropertyType::Usage:	pProp = new ShapePropertyUsage( sName, sDescription ); break;
	default:
	    // problem
	    break;
    }

    if ( pProp != NULL )
	pProp->Configure( node );

    return pProp;
}
