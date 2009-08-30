#include "stdafx.h"
#include "shape.h"

#include "shapemcu.h"
#include "shapedirectionalswitch.h"
#include "shapecodedselectorswitch.h"
#include "shapeselectorswitch.h"
#include "shapelcd.h"
#include "shapekeymatrix.h"
#include "shapepotentiometer.h"
#include "shapedigitalencoder.h"
#include "shapeswitch.h"
#include "shapeled.h"
#include "shapebicolourled.h"
#include "shapetricolourled.h"
#include "shapergb.h"
#include "shapepwm.h"
#include "shapetimer.h"

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
        { "counter", Counter },
        { "pwm", PWM },
        { "power", Power },
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
	case ShapeType::AT90USB128:	    pShape = new ShapeMCU( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::DirSwitch:	    pShape = new ShapeDirectionalSwitch( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::KeyMatrix:	    pShape = new ShapeKeyMatrix( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::RotarySwitch:	    pShape = new ShapeSelectorSwitch( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::CodedRotarySwitch:  pShape = new ShapeCodedSelectorSwitch( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::LCD:		    pShape = new ShapeLCD( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::Pot:		    pShape = new ShapePotentiometer( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::DigitalEncoder:	    pShape = new ShapeDigitalEncoder( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::Switch:		    pShape = new ShapeSwitch( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::LED:		    pShape = new ShapeLED( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::BiColourLED:	    pShape = new ShapeBiColourLED( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::TriColourLED:	    pShape = new ShapeTriColourLED( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::RGB:		    pShape = new ShapeRGB( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::PWM:		    pShape = new ShapePWM( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::Counter:	    pShape = new ShapeTimer( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::Power:		    pShape = new Shape( node, sName, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	default:
	    assert( false );
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
	    bool bEnabled = XMLUtility::getAttribute( propertyNode, "enabled", true );
	    enum PropertyType::PropertyType ePropType = PropertyType::fromString( sPropType );

	    ShapeProperty *pShapeProperty = ShapeProperty::CreateShapeProperty( propertyNode, sPropName, sPropDescription, ePropType, bEnabled );

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
	    if ( pins[index]->wires().count() == 0 )
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
	    if ( pins[index]->wires().count() != 0 )
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
	if ( pin->wires().count() == 0 )
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
    PropertyValueUsage *pUsage = dynamic_cast<PropertyValueUsage*>( GetPropertyValue(sName,values) );
    if ( pUsage != NULL )
    {
	QStringList s = pUsage->Value.split(QChar(':'));
	if ( s.count() > 0 )
	    return s[0].toUShort();
    }
    return nDefault;
}

unsigned short Shape::GetPropertyValueUsage( const QString &sName, const QList<class PropertyValue *> &values, int nDefault ) const
{
    PropertyValueUsage *pUsage = dynamic_cast<PropertyValueUsage*>( GetPropertyValue(sName,values) );
    if ( pUsage != NULL )
    {
	QStringList s = pUsage->Value.split(QChar(':'));
	if ( s.count() > 1 )
	    return s[1].toUShort();
    }
    return nDefault;
}


void Shape::MakeDeviceXML( QDomElement &elem, int nCurrent, const QString &sPowerPin, const QList<PropertyValue *> &values  ) const
{
}

void Shape::MakeControlsXML( QDomElement &elem, const QList<class PinItem *> &pins, const QList<PropertyValue *> &values  ) const
{
}


void Shape::MakeDirectionalSwitchControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bPullup, bool bDebounce, const QString &sDirections, const QString &sNorthPort, const QString &sSouthPort, const QString &sEastPort, const QString &sWestPort, const QString &sNEPort, const QString &sNWPort, const QString &sSEPort, const QString &sSWPort ) const
{
    QDomElement node = elem.ownerDocument().createElement( "DirectionalSwitch" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Pullup", bPullup );
    XMLUtility::setAttribute( node, "Debounce", bDebounce );
    XMLUtility::setAttribute( node, "Directions", sDirections );
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

void Shape::MakeTricolourLEDControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bSource, const QString &sPort1, const QString &sPort2 ) const
{
    QDomElement node = elem.ownerDocument().createElement( "TricolourLED" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Source", bSource );
    XMLUtility::setAttribute( node, "PortA", sPort1 );
    XMLUtility::setAttribute( node, "PortB", sPort2 );
}


void Shape::MakeDigitalEncoderControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, const QString &sPortA, const QString &sPortB ) const
{
    QDomElement node = elem.ownerDocument().createElement( "DigitalEncoder" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "PortA", sPortA );
    XMLUtility::setAttribute( node, "PortB", sPortB );
}

void Shape::MakeKeyMatrixControl( QDomElement &elem, const QString &sNames, unsigned short nUsagePage, unsigned short nUsageMin, bool bPullUp, bool bDebounce, const QStringList &rows, const QStringList &cols  ) const
{
    QDomElement node = elem.ownerDocument().createElement( "KeyMatrix" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "UsageMin", nUsageMin );
    XMLUtility::setAttribute( node, "Pullup", bPullUp );
    XMLUtility::setAttribute( node, "Debounce", bDebounce );

    foreach ( QString sRow, rows )
    {
	QDomElement row = elem.ownerDocument().createElement( "RowIn" );
	node.appendChild( row );
	XMLUtility::setAttribute( row, "Port", sRow );
    }

    foreach ( QString sCol, cols )
    {
	QDomElement col = elem.ownerDocument().createElement( "ColOut" );
	node.appendChild( col );
	XMLUtility::setAttribute( col, "Port", sCol );
    }

    // Names = [r,c]String[r,c]String...
    QRegExp rx("\\[(\\d+),(\\d+)\\]([^\\[]+)");
    int pos = 0;

    while ((pos = rx.indexIn(sNames, pos)) != -1) 
    {
	int nRow = rx.cap(1).toInt();
	int nCol = rx.cap(2).toInt();
	QString sName = rx.cap(3);

	if ( nRow >= 0 && nRow < rows.count() &&
	     nCol >= 0 && nCol < cols.count() )
	{
	    QDomElement key = elem.ownerDocument().createElement( "Key" );
	    node.appendChild( key );
	    XMLUtility::setAttribute( key, "Row", nRow );
	    XMLUtility::setAttribute( key, "Col", nCol );
	    XMLUtility::setAttribute( key, "Name", sName );
	}

	pos += rx.matchedLength();
    }
}

void Shape::MakeRotarySwitchControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bPullUp, bool bDebounce, bool bEncoded, int nBits, const QStringList &outputs ) const
{
    QDomElement node = elem.ownerDocument().createElement( "RotarySwitch" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Pullup", bPullUp );
    XMLUtility::setAttribute( node, "Debounce", bDebounce );
    XMLUtility::setAttribute( node, "Encoded", bEncoded );
    if ( bEncoded )
	XMLUtility::setAttribute( node, "Outputs", nBits );

    for ( int i = 0; i < outputs.count(); i++ )
    {
	QDomElement input = elem.ownerDocument().createElement( "Input" );
	node.appendChild( input );
	XMLUtility::setAttribute( input, "Bit", bEncoded ? (1<<i) : (i+1) );
	XMLUtility::setAttribute( input, "Port", outputs[i] );
    }
}

void Shape::MakeLCDControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nRows, int nCols, bool b8Bit,
			    const QString &sPortRS, const QString &sPortRW, const QString &sPortE, const QString &sPortD0, const QString &sPortD1, const QString &sPortD2,
			    const QString &sPortD3, const QString &sPortD4, const QString &sPortD5, const QString &sPortD6, const QString &sPortD7, 
			    int nAddrRow0, int nAddrRow1, int nAddrRow2, int nAddrRow3 ) const
{
    QDomElement node = elem.ownerDocument().createElement( "LCD" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Rows", nRows );
    XMLUtility::setAttribute( node, "Columns", nCols );
    XMLUtility::setAttribute( node, "Bus8Bit", b8Bit );
    XMLUtility::setAttribute( node, "PortRS", sPortRS);
    XMLUtility::setAttribute( node, "PortRW", sPortRW);
    XMLUtility::setAttribute( node, "PortE", sPortE);
    XMLUtility::setAttribute( node, "PortD0", sPortD0);
    XMLUtility::setAttribute( node, "PortD1", sPortD1);
    XMLUtility::setAttribute( node, "PortD2", sPortD2);
    XMLUtility::setAttribute( node, "PortD3", sPortD3);
    XMLUtility::setAttribute( node, "PortD4", sPortD4);
    XMLUtility::setAttribute( node, "PortD5", sPortD5);
    XMLUtility::setAttribute( node, "PortD6", sPortD6);
    XMLUtility::setAttribute( node, "PortD7", sPortD7);
    XMLUtility::setAttribute( node, "Row0", nAddrRow0);
    XMLUtility::setAttribute( node, "Row1", nAddrRow1);
    XMLUtility::setAttribute( node, "Row2", nAddrRow2);
    XMLUtility::setAttribute( node, "Row3", nAddrRow3);
}


void Shape::MakeLEDControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, const QString &sPort, bool bSource ) const
{
    QDomElement node = elem.ownerDocument().createElement( "LED" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Port", sPort );
    XMLUtility::setAttribute( node, "Source", bSource );
}

void Shape::MakeBiColourControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, const QString &sPortA, const QString &sPortB ) const
{
    QDomElement node = elem.ownerDocument().createElement( "BicolourLED" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "PortA", sPortA );
    XMLUtility::setAttribute( node, "PortB", sPortB );
}

void Shape::MakeRGBControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, bool bSource, const QString &sPortR, const QString &sPortG, const QString &sPortB ) const
{
    QDomElement node = elem.ownerDocument().createElement( "RGBLED" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Source", bSource );
    XMLUtility::setAttribute( node, "PortR", sPortR );
    XMLUtility::setAttribute( node, "PortG", sPortG );
    XMLUtility::setAttribute( node, "PortB", sPortB );
}

void Shape::MakePWMControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nResolution, const QString &sPort ) const
{
    QDomElement node = elem.ownerDocument().createElement( "PWM" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Resolution", nResolution );
    XMLUtility::setAttribute( node, "Port", sPort );
}

void Shape::MakeCounterControl( QDomElement &elem, const QString &sName, unsigned short nUsagePage, unsigned short nUsage, int nPeriod, int nBits ) const
{
    QDomElement node = elem.ownerDocument().createElement( "Counter" );
    elem.appendChild( node );

    XMLUtility::setAttribute( node, "Name", sName );
    XMLUtility::setAttribute( node, "UsagePage", nUsagePage );
    XMLUtility::setAttribute( node, "Usage", nUsage );
    XMLUtility::setAttribute( node, "Period", nPeriod );
    XMLUtility::setAttribute( node, "Bits", nBits );
}

QString Shape::GetPort( QList<PinItem *> pins, const QString &sName  ) const
{
    for ( int i = 0; i < pins.count(); i++ )
	if ( pins[i]->pin()->id().compare( sName, Qt::CaseInsensitive ) == 0 )
	{
	    if ( pins[i]->wires().count() == 0 )
	    {
		//assert( false );
		//return QString();
	    }
	    else if ( pins[i]->wires()[0]->pin1() != pins[i] )
	    {
		assert( pins[i]->wires().count() == 1 );
		if ( pins[i]->wires().count() == 1 )
		    return pins[i]->wires()[0]->pin1()->pin()->id();
	    }
	    else
	    {
		assert( pins[i]->wires().count() == 1 );
		if ( pins[i]->wires().count() == 1 )
		    return pins[i]->wires()[0]->pin2()->pin()->id();
	    }
	    break;
	}
    return QString();
}


void Shape::populateProperties( QList<PropertyValue *> &values ) const
{
    m_Properties.populate( values );
}

void Shape::retrieveProperties( QList<PropertyValue *> &values ) const
{
    m_Properties.retrieve( values );
}
