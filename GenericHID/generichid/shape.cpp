#include "stdafx.h"
#include "shape.h"

#include "shapemcu.h"
#include "shapedirectionalswitch.h"
#include "shapecodedselectorswitch.h"
#include "shapeselectorswitch.h"
#include "shapelcd.h"
#include "shapekeymatrix.h"

#include "shapeproperty.h"
#include "shapepropertybool.h"
#include "shapepropertyenum.h"
#include "shapepropertyint.h"
#include "shapepropertystring.h"
#include "shapepropertyusage.h"


namespace ShapeType
{
    struct ShapeTypeDetails
    {
	const char *sName;
	enum ShapeType eType;
    } types[] = 
    {
	{ "AT90USB128", AT90USB128 },
	{ "pot", Pot },
	{ "digitalencoder", DigitalEncoder },
        { "switch", Switch },
        { "keymatrix", KeyMatrix },
        { "rotaryswitch", RotarySwitch },
        { "codedrotaryswitch", CodedRotarySwitch },
        { "lcd", LCD },
        { "led", LED },
        { "rgbled", RGB },
        { "bicolourled", BiColourLED },
        { "tricolourled", TriColourLED },
        { "dirswitch", DirSwitch },
        { "timer", Timer },
        { "pwm", PWM },
    };

    enum ShapeType fromString( const QString &s )
    {
	for ( unsigned int i = 0; i < countof(types); i++ )
	    if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		return types[i].eType;
	return None;
    }

    const char * toString( enum ShapeType eType )
    {
	for ( unsigned int i = 0; i < countof(types); i++ )
	    if ( types[i].eType == eType )
		return types[i].sName;
	return "None";
    }
}

Shape::Shape(QDomElement & /*shapeNode*/, const QString &sShapeName, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: m_sShapeName( sShapeName )
, m_eShapeType(eShapeType)
, m_sShapeId(sShapeId)
, m_bSource(bSource)
, m_sImageFile(sImageFile)
, m_nImageWidth(nImageWidth)
, m_nImageHeight(nImageHeight)
, m_sIconFile(sIconFile)
, m_nMaxInstances(nMaxInstances)
, m_sDescription(sDescription)
, m_Properties( sShapeName, sDescription )
{
}

Shape::~Shape(void)
{
}


Shape *Shape::CreateFromXML( QDomElement &node )
{
    // common properties
    QString sShapeId = XMLUtility::getAttribute( node, "Id", "" );
    QString sType = XMLUtility::getAttribute( node, "Type", "" );
    bool bSource = XMLUtility::getAttribute( node, "Source", false);
    QString sImageFile = XMLUtility::getAttribute( node, "Image", "" );
    QString sIconFile = XMLUtility::getAttribute( node, "Icon", "" );
    int nImageWidth = XMLUtility::getAttribute( node, "width", 0 );
    int nImageHeight = XMLUtility::getAttribute( node, "height", 0 );
    int nMaxInstances = XMLUtility::getAttribute( node, "maxInstances", 0 );
    QString sName = XMLUtility::getAttribute( node, "name", "" );
    QString sDescription = XMLUtility::getAttribute( node, "description", "" );


    // Create the component
    ShapeType::ShapeType eShapeType = ShapeType::fromString( sType );
    Shape *pShape = NULL;
    switch ( eShapeType )
    {
	case ShapeType::AT90USB128:	    pShape = new ShapeMCU(node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::DirSwitch:	    pShape = new ShapeDirectionalSwitch(node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::KeyMatrix:	    pShape = new ShapeKeyMatrix(node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::RotarySwitch:	    pShape = new ShapeSelectorSwitch(node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::CodedRotarySwitch:  pShape = new ShapeCodedSelectorSwitch(node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::LCD:		    pShape = new ShapeLCD(node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::Pot:		    
	case ShapeType::DigitalEncoder:
	case ShapeType::Switch:
	case ShapeType::LED:
	case ShapeType::BiColourLED:
	case ShapeType::TriColourLED:
	case ShapeType::RGB:	    
	case ShapeType::PWM:	    
	case ShapeType::Timer:		    pShape = new Shape(node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	default:
	    // Problem
	    break;
    }

    if ( pShape != NULL )
    {
	// the connection pins for the component
	QDomNodeList pinNodes = XMLUtility::elementsByTagName( node, "pin" );
	for ( uint i = 0; i < pinNodes.length(); i++ )
	{
	    QDomElement pinNode = pinNodes.item(i).toElement();
	    Pin *pPin = Pin::CreateFromXML( pinNode, pShape );

	    if ( pPin != NULL )
		pShape->m_Pins.push_back( pPin );
	}

	// the shapes configurable properties
	QDomNodeList propertyNodes = XMLUtility::elementsByTagName( node, "property" );
	for ( uint i = 0; i < propertyNodes.length(); i++ )
	{
	    QDomElement propertyNode = propertyNodes.item(i).toElement();

	    QString sPropName = XMLUtility::getAttribute( propertyNode, "name", "" );
	    QString sPropDescription = XMLUtility::getAttribute( propertyNode, "description", "" );
	    QString sPropType = XMLUtility::getAttribute( propertyNode, "type", "" );
	    enum PropertyType::PropertyType ePropType = PropertyType::fromString( sPropType );

	    ShapeProperty *pShapeProperty = ShapeProperty::CreateShapeProperty( propertyNode, sPropName, sPropDescription, ePropType );

	    if ( pShapeProperty != NULL )
		pShape->m_Properties.add( pShapeProperty );
	}
    }
    return pShape;
}

QPixmap Shape::pixmap()
{
    if ( m_pixmap.isNull() )
	m_pixmap.load( m_sImageFile );
    return m_pixmap;
}


bool Shape::CheckPins( QString &sErrors, const QList<class PinItem *> &pins, const QSet<QString> &pinsToCheck ) const
{
    bool bSuccess = true;

    foreach ( const QString sPin, pinsToCheck )
    {
	int index;
	if ( (index = findPin( sPin )) >= 0 )
	{
	    if ( pins[index]->wire() == NULL )
	    {
		bSuccess = false;
		sErrors += QString( "Pin '%1' is not connected on '%2'\n" ).arg(pins[index]->pin()->id()).arg(m_sShapeName);
	    }
	}
    }
    return bSuccess;
}


bool Shape::CheckNotPins( QString &sErrors, const QList<class PinItem *> &pins, const QSet<QString> &pinsToCheck ) const
{
    bool bSuccess = true;

    foreach ( const QString sPin, pinsToCheck )
    {
	int index;
	if ( (index = findPin( sPin )) >= 0 )
	{
	    if ( pins[index]->wire() != NULL )
	    {
		bSuccess = false;
		sErrors += QString( "Pin '%1' is connected although it is not used on '%2'\n" ).arg(pins[index]->pin()->id()).arg(m_sShapeName);
	    }
	}
    }
    return bSuccess;
}


int Shape::findPin( const QString &sName ) const
{
    for ( int i = 0; i < m_Pins.count(); i++ )
	if ( m_Pins[i]->id().compare( sName, Qt::CaseInsensitive ) == 0 )
	    return i;
    return -1;
}


bool Shape::Verify( QString &sErrors, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values ) const
{
    bool bSuccess = true;

    // default implmentation just checks that every pin is connected.  Shapes that don't need this can override
    foreach ( PinItem *pin, pins )
    {
	if ( pin->wire() == NULL )
	{
	    sErrors += QString( "Pin '%1' is not connected on '%2'\n" ).arg(pin->pin()->id()).arg(m_sShapeName);
	    bSuccess = false;
	}
    }
    return bSuccess;
}



PropertyValue *Shape::GetPropertyValue( const QString &sName, const QList<class PropertyValue *> &values ) const
{
    // Find the index of the property
    int index = m_Properties.find(sName);
    if ( index < 0 || index >= values.count() )
	return NULL;
    else
	return values[index];
}


bool Shape::GetPropertyValueBool( const QString &sName, const QList<class PropertyValue *> &values, bool bDefault ) const
{
    // Look up and convert the value
    PropertyValueBool *pBool = dynamic_cast<PropertyValueBool *>( GetPropertyValue(sName,values) );
    if ( pBool == NULL )
	return bDefault;
    else
	return pBool->Value;
}


QString Shape::GetPropertyValueEnum( const QString &sName, const QList<class PropertyValue *> &values, const QString &sDefault ) const
{
    // Look up and convert the value
    PropertyValueEnum *pEnum = dynamic_cast<PropertyValueEnum *>( GetPropertyValue(sName,values) );
    if ( pEnum == NULL )
	return sDefault;
    else
	return pEnum->Value;
}


int Shape::GetPropertyValueInt( const QString &sName, const QList<class PropertyValue *> &values, int nDefault ) const
{
    // Look up and convert the value
    PropertyValueInt *pInt = dynamic_cast<PropertyValueInt*>( GetPropertyValue(sName,values) );
    if ( pInt == NULL )
	return nDefault;
    else
	return pInt->Value;
}

QString Shape::GetPropertyValueString( const QString &sName, const QList<class PropertyValue *> &values, const QString &sDefault ) const
{
    PropertyValueString *pString = dynamic_cast<PropertyValueString*>( GetPropertyValue(sName,values) );
    if ( pString == NULL )
	return sDefault;
    else
	return pString->Value;
}

unsigned short Shape::GetPropertyValueUsagePage( const QString &sName, const QList<class PropertyValue *> &values, int nDefault ) const
{
    QString sUsage = GetPropertyValueString(sName, values, "" );
    QStringList s = sUsage.split(QChar(':'));
    if ( s.count() > 0 )
	return s[0].toUShort();
    else 
	return nDefault;
}

unsigned short Shape::GetPropertyValueUsage( const QString &sName, const QList<class PropertyValue *> &values, int nDefault ) const
{
    QString sUsage = GetPropertyValueString(sName, values, "" );
    QStringList s = sUsage.split(QChar(':'));
    if ( s.count() > 1 )
	return s[1].toUShort();
    else 
	return nDefault;
}


void Shape::MakeDeviceXML( QDomElement &elem, int nCurrent, const QList<PropertyValue *> &values  ) const
{
}

void Shape::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
}


void Shape::MakeDirectionalSwitchControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bPullup, bool bDebounce, int nDirections, const QString &sNorthPort, const QString &sSouthPort, const QString &sEastPort, const QString &sWestPort, const QString &sNEPort, const QString &sNWPort, const QString &sSEPort, const QString &sSWPort ) const
{
    QDomElement node = elem.ownerDocument().createElement( "DirectionalSwitch" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Pullup", bPullup );
    XMLUtility::setAttribute( node, "Debounce", bDebounce );
    XMLUtility::setAttribute( node, "Directons", nDirections );
    if ( !sNorthPort.isEmpty() ) XMLUtility::setAttribute( node, "PortN", sNorthPort );
    if ( !sSouthPort.isEmpty() ) XMLUtility::setAttribute( node, "PortS", sSouthPort );
    if ( !sEastPort.isEmpty() ) XMLUtility::setAttribute( node, "PortE", sEastPort );
    if ( !sWestPort.isEmpty() ) XMLUtility::setAttribute( node, "PortW", sWestPort );
    if ( !sNEPort.isEmpty() ) XMLUtility::setAttribute( node, "PortNE", sNEPort );
    if ( !sNWPort.isEmpty() ) XMLUtility::setAttribute( node, "PortNW", sNWPort );
    if ( !sSEPort.isEmpty() ) XMLUtility::setAttribute( node, "PortSE", sSEPort );
    if ( !sSWPort.isEmpty() ) XMLUtility::setAttribute( node, "PortSW", sSWPort );
}

void Shape::MakePotentiometerControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nBits, const QString &sPort ) const
{
    QDomElement node = elem.ownerDocument().createElement( "AnalogIn" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Bits", nBits );
    XMLUtility::setAttribute( node, "Port", sPort );
}

void Shape::MakeSwitchControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bPullup, bool bDebounce, const QString &sPort ) const
{
    QDomElement node = elem.ownerDocument().createElement( "Switch" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Pullup", bPullup );
    XMLUtility::setAttribute( node, "Debounce", bDebounce );
    XMLUtility::setAttribute( node, "Port", sPort );
}

void Shape::MakeTricolourLEDControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, const QString &sPort1, const QString &sPort2 ) const
{
    QDomElement node = elem.ownerDocument().createElement( "TricolourLED" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "PortA", sPort1 );
    XMLUtility::setAttribute( node, "PortB", sPort2 );
}
