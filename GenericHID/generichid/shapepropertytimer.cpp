#include "stdafx.h"
#include "shapepropertytimer.h"

ShapePropertyTimer::ShapePropertyTimer( QtStringPropertyManager *manager, const QString &sName, const QString &sDescription, bool bEnabled )
: ShapePropertyString( sName, sDescription, bEnabled )
, m_pManager( manager )
{
}

ShapePropertyTimer::~ShapePropertyTimer(void)
{
}

QtProperty *ShapePropertyTimer::getQtProperty()
{
    QtProperty *pProp = m_pManager->addProperty( m_sName );
    pProp->setToolTip( m_sDescription );
    pProp->setWhatsThis( m_sDescription );
    pProp->setEnabled( m_bEnabled );
    return pProp;
}


void ShapePropertyTimer::setValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueString *val = dynamic_cast<PropertyValueString *>( propVal );
    if ( val != NULL )
	m_pManager->setValue( qtProp, val->Value );
}

void ShapePropertyTimer::getValue(QtProperty *qtProp, PropertyValue *propVal) const
{
    PropertyValueString *val = dynamic_cast<PropertyValueString *>( propVal );
    if ( val != NULL )
	val->Value = m_pManager->value( qtProp );
}

