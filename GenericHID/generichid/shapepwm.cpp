#include "stdafx.h"
#include "shapepwm.h"
#include "shapeitem.h"
#include "timercounter.h"
#include "shapepropertystring.h"
#include "shapepropertyint.h"
#include "shapepropertyenum.h"


ShapePWM::ShapePWM(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapePWM::~ShapePWM(void)
{
}


void ShapePWM::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    MakePWMControl( elem, 
		    GetPropertyValueString("Name",values,""), 
		    GetPropertyValueUsagePage("Usage",values,1),
		    GetPropertyValueUsage("Usage",values,1),
		    GetPropertyValueInt("Resolution",values,1),
		    GetPort( pins, "PWM") );
}


void ShapePWM::wireAddedEvent( WireItem *pWire, QList<PropertyValue *> &values ) const
{
    // check the pin we are connected to.  It must be an MCU pwm pin.
    // Find the timer, freq, resolution
    PinItem *pMCUPin;
    if ( pWire->pin1()->parentShape()->shapeData()->shapeType() == ShapeType::PWM )
	pMCUPin = pWire->pin2();
    else
	pMCUPin = pWire->pin1();

    UpdateTimerDetails( pMCUPin, values );
}

void ShapePWM::UpdateTimerDetails( PinItem *pMCUPin, QList<PropertyValue *> &values ) const
{
    // Need to read the properties
    QString sPinName = pMCUPin->pin()->id();
    int nTimer;
    if ( sPinName.compare( "PB4", Qt::CaseInsensitive ) == 0 ||
	 sPinName.compare( "PD1", Qt::CaseInsensitive ) == 0 )
	 nTimer = 2;
    else if ( sPinName.compare( "PB5", Qt::CaseInsensitive ) == 0 ||
	      sPinName.compare( "PB6", Qt::CaseInsensitive ) == 0 ||
	      sPinName.compare( "PB7", Qt::CaseInsensitive ) == 0 )
	nTimer = 1;
    else
	nTimer = 3;

    QString sTimerDetails = pMCUPin->parentShape()->GetPropertyValueString( QString("Timer%1").arg(nTimer), "1,8,100" );
    if ( !sTimerDetails.isEmpty() )
    {
	int nMode, nPrescaler, nTop;
	TimerCounter::ExtractDetails( sTimerDetails, nMode, nPrescaler, nTop );

	// Set value
	PropertyValue *valueFrequency = GetPropertyValue( "Frequency", values );
	PropertyValueString *strValue = dynamic_cast<PropertyValueString *>( valueFrequency );
	strValue->Value = TimerCounter::MakeFreq( TimerCounter::MakeFreq(nMode,nPrescaler,nTop) );

	// set current value
	QString s = TimerCounter::MakeFreq( TimerCounter::MakeFreq(nMode,nPrescaler,nTop) );
	ShapeProperty::m_stringManager->setValue( m_Properties.findQt( "Frequency" ), s );
 
	// set value
	PropertyValue *valueResolution = GetPropertyValue( "Resolution", values );
	PropertyValueInt *intValue = dynamic_cast<PropertyValueInt *>( valueResolution );
	intValue->Value = nTop;

	// set current value
	ShapeProperty::m_intManager->setValue( m_Properties.findQt( "Resolution" ), nTop );

	PropertyValue *valueTimer = GetPropertyValue( "Timer", values );
	PropertyValueEnum *enumValue = dynamic_cast<PropertyValueEnum *>( valueTimer );
	enumValue->Value = QString("Timer%1").arg(nTimer);

	ShapeProperty::m_enumManager->setValue( m_Properties.findQt( "Timer" ), nTimer-1 );
    }
}


void ShapePWM::UpdateTimerDetails( ShapeItem *pItem ) const
{
    int nPinIndex = findPin( "PWM" );
    PinItem *pPin = pItem->pins()[nPinIndex];

    if ( pPin->wires().count() == 1 )	// can only be one
    {
	WireItem *pWire = pPin->wires()[0];

	// check the pin we are connected to.  It must be an MCU pwm pin.
	PinItem *pMCUPin;
	if ( pWire->pin1() == pPin )
	    pMCUPin = pWire->pin2();
	else
	    pMCUPin = pWire->pin1();

	UpdateTimerDetails( pMCUPin, pItem->values() );
    }
}


