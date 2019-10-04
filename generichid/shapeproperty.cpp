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
#include "shapeproperty.h"
#include "shapepropertybool.h"
#include "shapepropertyenum.h"
#include "shapepropertyint.h"
#include "shapepropertystring.h"
#include "shapepropertytimer.h"
#include "shapepropertyusage.h"
#include "shapepropertyrcnames.h"
#include "listeditorfactory.h"
#include "timereditorfactory.h"
#include "keymatrixeditorfactory.h"
#include "timercounter.h"


namespace  PropertyType
{
    struct PropertyTypeMap
    {
	enum PropertyType eType;
	const char *sName;
    } types[] = 
    {
	{ String, "String" },
        { Enum, "Enum" },
        { Bool, "Bool" },
        { Int, "Int" },
        { RCNames, "keynames" },
	{ Usage, "Usage" },
	{ Timer13, "Timer13" },
	{ Timer2, "Timer2" },
	{ Timer2, "Timer2" },
    };

    enum PropertyType fromString( const QString &s )
    {
	for ( unsigned int i = 0; i < countof(types); i++ )
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
TimerStringPropertyManager *ShapeProperty::m_timer13Manager = NULL;
TimerStringPropertyManager *ShapeProperty::m_timer2Manager = NULL;
KeyMatrixStringPropertyManager *ShapeProperty::m_keyMatrixNameManager = NULL;

KeyMatrixEditorFactory *ShapeProperty::m_keyMatrixNameFactory = NULL;

bool ShapeProperty::m_bInitialised = false;

QtProperty *ShapeProperty::MakeGroupItem( const QString &s )
{
    Init();
    return m_groupManager->addProperty(s);
}

void ShapeProperty::SetBrowserFactory( QtAbstractPropertyBrowser *browser, unsigned int nCpuClock )
{
    Init();

    QtLineEditFactory *lineEditFactory = new QtLineEditFactory();
    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory();
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory();
    ListEditorFactory *comboListFactory = new ListEditorFactory();
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory();
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory();
    TimerEditorFactory *timer13Factory = new TimerEditorFactory(16,QStringList()<< "1" << "8" << "64" << "256" << "1024", nCpuClock );
    TimerEditorFactory *timer2Factory = new TimerEditorFactory(8,QStringList()<< "1" << "8" << "32" << "64" << "128" << "256" << "1024", nCpuClock );
    TimerCounter::SetClock( nCpuClock );
    m_keyMatrixNameFactory  = new KeyMatrixEditorFactory();

    browser->setFactoryForManager(m_stringManager, lineEditFactory);
    browser->setFactoryForManager(m_timer13Manager, timer13Factory);
    browser->setFactoryForManager(m_timer2Manager, timer2Factory);
    browser->setFactoryForManager(m_keyMatrixNameManager, m_keyMatrixNameFactory );
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
	m_timer13Manager  = new TimerStringPropertyManager();
	m_timer2Manager  = new TimerStringPropertyManager();
	m_keyMatrixNameManager = new KeyMatrixStringPropertyManager();
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



ShapeProperty::ShapeProperty( const QString &sName, const QString &sDescription, bool bEnabled )
: m_sName(sName)
, m_sDescription( sDescription )
, m_bEnabled( bEnabled )
{
    Init();
}

ShapeProperty::~ShapeProperty(void)
{
}


ShapeProperty *ShapeProperty::CreateShapeProperty( QDomElement &node, const QString &sName, const QString &sDescription, PropertyType::PropertyType ePropType, bool bEnabled, QString &sError )
{
    ShapeProperty *pProp = NULL;

    switch ( ePropType )
    {
	case PropertyType::String:	pProp = new ShapePropertyString( sName, sDescription, bEnabled ); break;
        case PropertyType::Enum:	pProp = new ShapePropertyEnum( sName, sDescription, bEnabled ); break;
        case PropertyType::Bool:	pProp = new ShapePropertyBool( sName, sDescription, bEnabled ); break;
        case PropertyType::Int:		pProp = new ShapePropertyInt( sName, sDescription, bEnabled ); break;
        case PropertyType::Usage:	pProp = new ShapePropertyUsage( sName, sDescription, bEnabled ); break;
        case PropertyType::Timer13:	pProp = new ShapePropertyTimer( m_timer13Manager, sName, sDescription, bEnabled ); break;
        case PropertyType::Timer2:	pProp = new ShapePropertyTimer( m_timer2Manager, sName, sDescription, bEnabled ); break;
        case PropertyType::RCNames:	pProp = new ShapePropertyRCNames( sName, sDescription, bEnabled ); break;
	default:
	    sError = QString( "Unknown property type on line %1 of config file" ).arg(node.lineNumber());
	    break;
    }

    if ( pProp != NULL )
	pProp->Configure( node );

    return pProp;
}


QDomElement ShapeProperty::WriteXML(QDomElement &node, PropertyValue *) const
{
    QDomElement propNode = node.ownerDocument().createElement( "Property" );
    node.appendChild( propNode );

    XMLUtility::setAttribute( propNode, "name", m_sName );
    return propNode;
}
