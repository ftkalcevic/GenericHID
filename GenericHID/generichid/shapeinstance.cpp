#include "stdafx.h"
#include "shapeinstance.h"
#include "pinitem.h"
#include "shapeitem.h"

ShapeInstance::ShapeInstance(const Shape *pShape)
: m_pShape(pShape)
{
}

ShapeInstance::~ShapeInstance(void)
{
}


bool ShapeInstance::CreateGraphics(Editor *pEditor)
{
    m_pItem = new ShapeItem( m_pShape->shapeFile(), pEditor, NULL );

    m_pItem->setZValue(-1);
    m_pItem->setFlag( QGraphicsItem::ItemIsMovable);
    m_pItem->setFlag( QGraphicsItem::ItemIsSelectable);
    m_pItem->setBoundingRegionGranularity(0);

    // Create the pins
    foreach (Pin *pPin, m_pShape->pins() )
    {
	QRectF rc = pPin->geometry();
	PinItem *pPinItem = new PinItem( rc, m_pItem );
	if ( pPin->enabled() )
	    pPinItem->setBrush( QBrush(Qt::white) );
	else
	    pPinItem->setBrush( QBrush(Qt::lightGray) );
	pPinItem->setZValue(0);

	QGraphicsSimpleTextItem *pTextItem = new QGraphicsSimpleTextItem( pPin->id(), m_pItem );
	QPainter p;
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
