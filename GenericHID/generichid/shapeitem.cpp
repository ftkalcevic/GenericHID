#include "stdafx.h"
#include "shapeitem.h"
#include "pinitem.h"

ShapeItem::ShapeItem(const Shape *pShape, Editor *pEditor, QGraphicsItem *parent)
: QGraphicsPixmapItem(QPixmap(pShape->shapeFile()),parent)
, m_pShape( pShape )
, m_pEditor( pEditor )
, m_dRotate(0)
, m_bMirror(false)
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

