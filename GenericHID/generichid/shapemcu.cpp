#include "stdafx.h"
#include "shapemcu.h"
#include "shapeproperties.h"
#include "shapepropertybool.h"


ShapeMCU::ShapeMCU(QDomElement &node, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: Shape(node, sShapeName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription)
{
}

ShapeMCU::~ShapeMCU(void)
{
}


bool ShapeMCU::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<class PropertyValue *> &values ) const
{
    bool bSuccess = true;

    // A stock USB key can have stock joystick, leds, voltage monitor, and temp sensor enabled.
    // Make sure, if they are enabled, their pins aren't used.
    int index;
    if ( GetPropertyValueBool( "Use Voltage Monitor", values, false ) )
    {
	// We have the Use Voltage Monitor property.  Make sure there is nothing on the voltage monitor pin, PF3
	if ( (index = findPin( "PF3" )) >= 0 )
	{
	    if ( pins[index]->wire() != NULL )
	    {
		bSuccess = false;
		sErrors += "The 'Use Voltage Monitor' option has been set, but the pin, PF3, is used for something else";
	    }
	}
    }

    if ( GetPropertyValueBool( "Use Temperature Sensor", values, false ) )
    {
	// We have the Use Temperature Sensor property.  Make sure there is nothing on the voltage monitor pin, PF0
	if ( (index = findPin( "PF0" )) >= 0 )
	{
	    if ( pins[index]->wire() != NULL )
	    {
		bSuccess = false;
		sErrors += "The 'Use Temperature Sensor' option has been set, but the pin, PF0, is used for something else";
	    }
	}
    }

    if ( GetPropertyValueBool( "Use Joystick", values, false ) )
    {
	// We have the Use Use Joystick property.  Make sure there is nothing on the voltage monitor pin, PF0
	static const char *joystickPins[] = { "PE5", "PE4", "PB7", "PB6", "PB5" };

	int nErrorCount = 0;
	for ( int i = 0; i < countof(joystickPins); i++ )
	{
	    const char *sPin = joystickPins[i];
	    if ( (index = findPin( sPin )) >= 0 )
	    {
		if ( pins[index]->wire() != NULL )
		{
		    bSuccess = false;
		    if ( nErrorCount == 0 )
			sErrors += "The 'Use Joystick' option has been set, but...\n";
		    sErrors += QString("    the pin, %1, is used for something else").arg(sPin);
		    nErrorCount++;
		}
	    }
	}
    }

    // LEDs are a bit more tricky
    if ( GetPropertyValueBool( "Use LEDs", values, false ) )
    {
	// We have the Use LEDs property.  Make sure there is nothing on the LED pins, or the Use Status LEDs properties clash
	QString sEnum = GetPropertyValueEnum( "Use Status LEDs", values, "" );

	QStringList pinsToCheck;
	if ( sEnum == "Both" )
	{
	    bSuccess = false;
	    sErrors += "The 'Use LEDs' option cannot be selected with the 'Use Status LEDs' = 'Both'\n";
	}
	else if ( sEnum == "LED1" )
	{
	    // LED1 is used for status, so LED2 will be for us
	    pinsToCheck << "PD6" << "PD7";
	}
	else if ( sEnum == "LED2" )
	{
	    pinsToCheck << "PD4" << "PD5";
	}
	else
	    pinsToCheck << "PD6" << "PD7" << "PD4" << "PD5";

	int nErrorCount = 0;
	foreach ( QString sPin, pinsToCheck )
	{
	    if ( (index = findPin( sPin )) >= 0 )
	    {
		if ( pins[index]->wire() != NULL )
		{
		    bSuccess = false;
		    if ( nErrorCount == 0 )
			sErrors += "The 'Use LEDs' option has been set, but...\n";
		    sErrors += QString("    the pin, %1, is used for something else").arg(sPin);
		    nErrorCount++;
		}
	    }
	}
    }

    return bSuccess;
}

void ShapeMCU::MakeDeviceXML( QDomElement &elem, int nCurrent, const QList<PropertyValue *> &values  ) const
{
    QDomElement deviceNode = elem.ownerDocument().createElement( "Device" );
    elem.appendChild( deviceNode );
    QDomElement configNode = elem.ownerDocument().createElement( "Configuration" );
    elem.appendChild( configNode );

    XMLUtility::setAttribute( deviceNode, "VID", FRANKSWORKSHOP_VID );
    XMLUtility::setAttribute( deviceNode, "PID", GENERICHID_PID );
    XMLUtility::setAttribute( deviceNode, "Release", 100 );
    XMLUtility::setAttribute( deviceNode, "Manufacturer", "FranksWorkshop" );
    XMLUtility::setAttribute( deviceNode, "Product", GetPropertyValueString("Name",values,"") );
    XMLUtility::setAttribute( deviceNode, "SerialNo", GetPropertyValueString("SerialNo",values,"") );
    
    XMLUtility::setAttribute( configNode, "BusPowered", GetPropertyValueEnum("Voltage",values,"") != "Self-Powered" );	    // so bus is default
    XMLUtility::setAttribute( configNode, "PowerConsumption", nCurrent );
    XMLUtility::setAttribute( configNode, "5Volts", GetPropertyValueEnum("Voltage",values,"") == "5" );
    XMLUtility::setAttribute( configNode, "UseUSBKeyLEDs", GetPropertyValueEnum("Use Status LEDs",values,"") );
    XMLUtility::setAttribute( configNode, "UsagePage", GetPropertyValueUsagePage("Usage",values,1) );
    XMLUtility::setAttribute( configNode, "Usage", GetPropertyValueUsage("Usage",values,1) );
    XMLUtility::setAttribute( configNode, "SerialDebug", GetPropertyValueBool("Serial-Debug",values,false) );
    XMLUtility::setAttribute( configNode, "HIDDebug", GetPropertyValueBool("HID-Debug",values,false) );
    XMLUtility::setAttribute( configNode, "UseLEDs", GetPropertyValueBool("Use LEDs",values,false) );
}


void ShapeMCU::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
    bool bTempSensor = GetPropertyValueBool("Use Temperature Sensor",values,false);
    bool bVoltageMonitor = GetPropertyValueBool("Use Voltage Monitor",values,false);
    bool bUseJoystick = GetPropertyValueBool("Use Joystick",values,false);
    bool bUseLEDs  = GetPropertyValueBool("Use LEDs",values,false);
    QString sUseLEDs  = GetPropertyValueBool("Use LEDs",values,false);

    if ( bTempSensor )
    {
	// make an analog input on 
	MakePotentiometerControl( elem, "Temperature", 1, 1, 8, "PF0" );
    }
    if ( bVoltageMonitor )
    {
	// make an analog input on 
	MakePotentiometerControl( elem, "Voltage", 1, 1, 8, "PF3" );
    }
    if ( bUseJoystick )
    {
	// make a directional switch plus button
	MakeDirectionalSwitchControl( elem, "Joystick", 1, 1, true, true, 4, "PB7", "PE5", "PE4", "PB6", "", "", "", "" );
	MakeSwitchControl( elem, "JoystickButton", 9, 1, true, true, "PB5" );
    }
    if ( bUseLEDs )
    {
	if ( sUseLEDs.compare( "LED1", Qt::CaseSensitive) == 0 )
	{
	    // Make an tricolour-LED output on LED 2
	    MakeTricolourLEDControl( elem, "LED2", 8, 1000, "PD6", "PD7" );
	}
	else if ( sUseLEDs.compare( "LED2", Qt::CaseSensitive) == 0 )
	{
	    // Make an tricolour-LED output on LED 1
	    MakeTricolourLEDControl( elem, "LED1", 8, 1000, "PD4", "PD5" );
	}
	else if ( sUseLEDs.compare( "none", Qt::CaseSensitive ) == 0 )
	{
	    // Make an tricolour-LED output on LED 1 and 2
	    MakeTricolourLEDControl( elem, "LED1", 8, 1000, "PD4", "PD5" );
	    MakeTricolourLEDControl( elem, "LED2", 8, 1000, "PD6", "PD7" );
	}
    }
}