#include "stdafx.h"
#include "shapeitem.h"
#include "pinitem.h"

ShapeItem::ShapeItem(const Shape *pShape, int id, Editor *pEditor, QGraphicsItem *parent)
: QGraphicsPixmapItem(QPixmap(pShape->shapeFile()),parent)
, m_pShape( pShape )
, m_pEditor( pEditor )
, m_dRotate(0)
, m_bMirror(false)
, m_nId( id )
{
    setAcceptHoverEvents( true );
    CreateProperties();
    CreateGraphics();
}

ShapeItem::~ShapeItem()
{
}

void ShapeItem::CreateProperties()
{
    foreach ( PropertyValue *v, m_values )
	delete v;
    m_values.clear();
    foreach ( const ShapeProperty *prop, m_pShape->properties().items() )
    {
	m_values.append( prop->createValue() );
    }
}

QVariant ShapeItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
    if ( change == QGraphicsItem::ItemPositionHasChanged )
        emit itemChange( this, change, value );

    return QGraphicsPixmapItem::itemChange(change, value);
}


bool ShapeItem::CreateGraphics()
{
    setZValue(-1);
    setFlag( QGraphicsItem::ItemIsMovable);
    setFlag( QGraphicsItem::ItemIsSelectable);
    setBoundingRegionGranularity(0);

    // Create the pins
    foreach (Pin *pPin, m_pShape->pins() )
    {
	QRectF rc = pPin->geometry();
	PinItem *pPinItem = new PinItem( rc, pPin, m_pEditor, this );
	if ( pPin->enabled() )
	    pPinItem->setBrush( QBrush(Qt::white) );
	else
	    pPinItem->setBrush( QBrush(Qt::lightGray) );
	pPinItem->setZValue(0);
	QString sToolTip;
	if ( pPin->enabled() )
	{
	    sToolTip = pPin->id();
	    if ( pPin->pinType() != PinType::None )
	    {
		sToolTip += ":";
		sToolTip += PinType::toString( pPin->pinType() );
	    }
	}
	else 
	{
	    if ( !pPin->otherUse().isEmpty() )
		sToolTip = QString("Used by: ") + pPin->otherUse();
	}
	if ( !sToolTip.isEmpty() )
	    pPinItem->setToolTip( sToolTip );
	m_pins.push_back( pPinItem );

	QGraphicsSimpleTextItem *pTextItem = new QGraphicsSimpleTextItem( pPin->id(), this );
	QSize rawSize = QApplication::fontMetrics().boundingRect(pPin->id()).size();

	QTransform rotateTransform;
	rotateTransform.rotate( pPin->rotate() );
	QPointF rotated = rotateTransform.map( QPointF(rawSize.width(),rawSize.height()) );
	QSize size( rotated.x(), rotated.y() );

	QPointF pos;
	switch ( pPin->hAlign() )
	{
	    case PinHAlign::Left:	    pos.setX( rc.left() ); break;
	    case PinHAlign::Centre:	    pos.setX( rc.center().x() - size.width() / 2.0 ); break;
	    case PinHAlign::Right:	    pos.setX( rc.right() - size.width() ); break;
	    case PinHAlign::OutsideLeft:    pos.setX( rc.left() - size.width() ); break;
	    case PinHAlign::OutsideRight:   pos.setX( rc.right() ); break;
	}
	switch ( pPin->vAlign() )
	{
	    case PinVAlign::Top:    pos.setY( rc.top() ); break;
	    case PinVAlign::Centre: pos.setY( rc.center().y() - size.height() / 2.0 ); break;
	    case PinVAlign::Bottom: pos.setY( rc.bottom() - size.height() ); break;
	    case PinVAlign::Above:  pos.setY( rc.top() - size.height() ); break;
	    case PinVAlign::Below:  pos.setY( rc.bottom() ); break;
	}
	pTextItem->rotate( pPin->rotate() );
	pTextItem->setPos( pos );
	pTextItem->setZValue(1);
    }

    return true;
}



//
//void ShapeItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
//{
//    unsetCursor();
//    QGraphicsPixmapItem::hoverLeaveEvent( event );
//}
//
//
//void ShapeItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
//{
//    if ( m_pEditor->m_eEditMode == EditMode::Mirror )
//	setCursor( *m_pEditor->m_curMirror );
//    else if ( m_pEditor->m_eEditMode == EditMode::Rotate )
//	setCursor( *m_pEditor->m_curRotate );
//    else 
//	unsetCursor();
//    QGraphicsPixmapItem::hoverEnterEvent( event );
//}
//
//
//void ShapeItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
//{
//    if ( m_pEditor->m_eEditMode == EditMode::Mirror )
//    {
//	m_bMirror = !m_bMirror;
//	DoTransform();
//    }
//    else if ( m_pEditor->m_eEditMode == EditMode::Rotate )
//    {
//	m_dRotate += 90.0;
//	while ( m_dRotate >= 360.0 )
//	    m_dRotate -= 360.0;
//	DoTransform();
//    }
//    QGraphicsPixmapItem::mousePressEvent( event );
//}

void ShapeItem::DoTransform()
{
    resetTransform();
    QPointF center = boundingRect().center();
    translate( center.x(), center.y() );
    rotate(m_dRotate );
    if ( m_bMirror )
	scale(-1,1);
    translate( -center.x(), -center.y() );
}


void ShapeItem::WriteXML( QDomElement &node ) const
{
    //	<Shapes>
    //	    <Shape ShapeId="strippedusbkey" InstanceId="a1be9021-6352-4820-a0ac-a6a9b4c22ecf" X="938" Y="666" Rotation="0" Mirror="False">
    //		<Property Name="Name" Value="" />
    QDomElement shapeNode = node.ownerDocument().createElement( "Shape" );
    node.appendChild( shapeNode );

    XMLUtility::setAttribute( shapeNode, "shapeId", m_pShape->id() );
    XMLUtility::setAttribute( shapeNode, "instanceId", m_nId );
    XMLUtility::setAttribute( shapeNode, "x", pos().x() );
    XMLUtility::setAttribute( shapeNode, "y", pos().y() );
    XMLUtility::setAttribute( shapeNode, "rotate", m_dRotate );
    XMLUtility::setAttribute( shapeNode, "mirror", m_bMirror );

    for ( int i = 0; i < m_pShape->properties().items().count(); i++ )
	m_pShape->properties().items()[i]->WriteXML( shapeNode, m_values[i] );
}


ShapeItem *ShapeItem::CreateFromXML( ShapeCollection *pCol, Editor *pEditor, QDomElement &shapeNode )
{
    QString sShapeId = XMLUtility::getAttribute( shapeNode, "shapeId", QString("") );
    int nId = XMLUtility::getAttribute( shapeNode, "instanceId", 0 );
    double x = XMLUtility::getAttribute( shapeNode, "x", 0.0 );
    double y = XMLUtility::getAttribute( shapeNode, "y", 0.0 );
    double dRotate = XMLUtility::getAttribute( shapeNode, "rotate", 0.0 );
    bool bMirror = XMLUtility::getAttribute( shapeNode, "mirror", false );

    ShapeItem *pShapeItem = NULL;
    const Shape *pShape = pCol->shape( sShapeId );
    if ( pShape != NULL )
    {
	pShapeItem = new ShapeItem( pShape, nId, pEditor, NULL );
	pShapeItem->setPos( QPointF(x,y) );
	pShapeItem->setRotation( dRotate );
	pShapeItem->setMirror( bMirror );

	QDomNodeList propertyNodes = XMLUtility::elementsByTagName( shapeNode, "Property" );
	for ( int i = 0; i < propertyNodes.count(); i++ )
	{
	    QDomElement item = propertyNodes.item(i).toElement();
	    QString sName = XMLUtility::getAttribute( item, "name", "" );
    	
	    for ( int p = 0; p < pShape->properties().items().count(); p++ )
	    {
		ShapeProperty *prop = pShape->properties().items()[p];
		if ( prop->name() == sName )
		{
		    prop->getXML( item, pShapeItem->m_values[p] );
		    break;
		}
	    }
	}
    }

    return pShapeItem;
}


bool ShapeItem::Verify( QString &sError ) const
{
    return m_pShape->Verify( sError, m_pins, m_values );
}

void ShapeItem::MakeDeviceXML( QDomElement &elem, int nCurrent, const QString &sPowerPin ) const 
{
    m_pShape->MakeDeviceXML( elem, nCurrent, sPowerPin, m_values );
}

void ShapeItem::MakeControlsXML( QDomElement &elem ) const 
{
    m_pShape->MakeControlsXML( elem, m_pins, m_values );
}

int ShapeItem::current() const 
{
    return m_pShape->GetPropertyValueInt( "Current", m_values, 0 );
}

void ShapeItem::PropertyChanged( QtBrowserItem * item )
{
    m_pShape->PropertyChanged( item, m_values );
}

void ShapeItem::populateProperties()
{
    m_pShape->populateProperties( m_values );
}
