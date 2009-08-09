#include "stdafx.h"
#include "shapeproperty.h"
#include "shapepropertybool.h"
#include "shapepropertyenum.h"
#include "shapepropertyint.h"
#include "shapepropertystring.h"
#include "shapepropertyusage.h"
#include "listeditorfactory.h"


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


QtGroupPropertyManager *ShapeProperty::m_groupManager = NULL;
QtStringPropertyManager *ShapeProperty::m_stringManager = NULL;
QtPointFPropertyManager *ShapeProperty::m_pointfScaleManager = NULL;
QtPointFPropertyManager *ShapeProperty::m_pointfManager = NULL;
QtDoublePropertyManager *ShapeProperty::m_doubleManager = NULL;
QtEnumPropertyManager *ShapeProperty::m_enumManager = NULL;
QtBoolPropertyManager *ShapeProperty::m_boolManager = NULL;
QtIntPropertyManager *ShapeProperty::m_intManager = NULL;
UsagePropertyManager *ShapeProperty::m_usageManager = NULL;

bool ShapeProperty::m_bInitialised = false;

QtProperty *ShapeProperty::MakeGroupItem( const QString &s )
{
    Init();
    return m_groupManager->addProperty(s);
}

void ShapeProperty::SetBrowserFactory( QtAbstractPropertyBrowser *browser )
{
    Init();

    QtLineEditFactory *lineEditFactory = new QtLineEditFactory();
    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory();
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory();
    ListEditorFactory *comboListFactory = new ListEditorFactory();
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory();
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory();

    browser->setFactoryForManager(m_stringManager, lineEditFactory);
    browser->setFactoryForManager(m_pointfManager->subDoublePropertyManager(), doubleSpinBoxFactory);
    browser->setFactoryForManager(m_pointfScaleManager->subDoublePropertyManager(), doubleSpinBoxFactory);
    browser->setFactoryForManager(m_doubleManager, doubleSpinBoxFactory);
    browser->setFactoryForManager(m_enumManager, comboBoxFactory);
    browser->setFactoryForManager(m_boolManager, checkBoxFactory);
    browser->setFactoryForManager(m_intManager, spinBoxFactory);
    browser->setFactoryForManager(m_usageManager->subListPropertyManager(), comboListFactory);
}

void ShapeProperty::Init()
{
    if ( !m_bInitialised )
    {
        m_groupManager = new QtGroupPropertyManager();
        m_stringManager = new QtStringPropertyManager();
        m_pointfScaleManager = new QtPointFPropertyManager();
        m_pointfManager = new QtPointFPropertyManager();
        m_doubleManager = new QtDoublePropertyManager();
        m_enumManager = new QtEnumPropertyManager();
        m_boolManager = new QtBoolPropertyManager();
	m_intManager = new QtIntPropertyManager();
        m_usageManager = new UsagePropertyManager();
        m_bInitialised = true;
    }
}



ShapeProperty::ShapeProperty( const QString &sName, const QString &sDescription )
: m_sName(sName)
, m_sDescription( sDescription )
{
    Init();
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
        case PropertyType::Usage:	pProp = new ShapePropertyUsage( sName, sDescription ); break;
	default:
	    // problem
	    break;
    }

    if ( pProp != NULL )
	pProp->Configure( node );

    return pProp;
}
