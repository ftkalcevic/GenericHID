#include "stdafx.h"
#include "shape.h"
#include "shapemcu.h"


namespace ShapeType
{
    struct 
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
        { "bicolourled", BiColourLED },
        { "tricolourled", TriColourLED },
        { "dirswitch", DirSwitch },
    };

    enum ShapeType fromString( const QString &s )
    {
	for ( int i = 0; i < countof(types); i++ )
	    if ( s.compare( types[i].sName, Qt::CaseInsensitive ) == 0 )
		return types[i].eType;
	return None;
    }

    const char * toString( enum ShapeType eType )
    {
	for ( int i = 0; i < countof(types); i++ )
	    if ( types[i].eType == eType )
		return types[i].sName;
	return "None";
    }
}

Shape::Shape(QDomElement &shapeNode, ShapeType::ShapeType eShapeType, const QString &sShapeId, bool bSource, const QString &sImageFile, int nImageWidth, int nImageHeight, const QString &sIconFile, int nMaxInstances, const QString &sDescription)
: m_eShapeType(eShapeType)
, m_sShapeId(sShapeId)
, m_bSource(bSource)
, m_sImageFile(sImageFile)
, m_nImageWidth(nImageWidth)
, m_nImageHeight(nImageHeight)
, m_sIconFile(sIconFile)
, m_nMaxInstances(nMaxInstances)
, m_sDescription(sDescription)
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
    QString sDescription = XMLUtility::getAttribute( node, "description", "" );


    // Create the component
    ShapeType::ShapeType eShapeType = ShapeType::fromString( sType );
    Shape *pShape = NULL;
    switch ( eShapeType )
    {
	case ShapeType::AT90USB128:	    pShape = new ShapeMCU(node, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
	case ShapeType::Pot:		    
	case ShapeType::DigitalEncoder:
	case ShapeType::Switch:
	case ShapeType::KeyMatrix:
	case ShapeType::RotarySwitch:
	case ShapeType::CodedRotarySwitch:
	case ShapeType::LCD:
	case ShapeType::LED:
	case ShapeType::BiColourLED:
	case ShapeType::TriColourLED:
	case ShapeType::DirSwitch:	    pShape = new Shape(node, eShapeType, sShapeId, bSource, sImageFile, nImageWidth, nImageHeight, sIconFile, nMaxInstances, sDescription);    break;
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
	    Pin *pPin = Pin::CreateFromXML( pinNode );

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
		pShape->m_Properties.push_back( pShapeProperty );
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

