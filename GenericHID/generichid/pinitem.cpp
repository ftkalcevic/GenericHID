#include "stdafx.h"
#include "pinitem.h"
#include "shape.h"
#include "shapeitem.h"

PinItem::PinItem( QRectF rc, const Pin *pPin, Editor *pEditor, QGraphicsItem *parent )
: QGraphicsRectItem( rc, parent )
, m_pEditor( pEditor )
, m_pPin( pPin )
, m_pWire( NULL )
{
    setAcceptHoverEvents( true );

}

PinItem::~PinItem()
{
}
//
//
//void PinItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
//{
//    ATLTRACE("PinItem::hoverEnterEvent\n");
//    if ( m_pEditor->m_eEditMode == EditMode::Wiring )
//    {
//	if ( m_pEditor->m_pWiringStartPin == NULL ) 
//	{
//	    // First Pin in the link
//	    if ( m_pWire != NULL )
//		setCursor( *m_pEditor->m_curWireNot );
//	    else
//		setCursor( *m_pEditor->m_curWire );
//	}
//	else
//	{
//	    // we are the second pin
//	    if ( m_pWire != NULL )  // already wired
//		setCursor( *m_pEditor->m_curWireNot );
//	    else
//	    {
//		// check compatibility
//		bool bGood = true;
//
//		// first the source/destination 
//		if ( (m_pPin->shape()->source() && m_pEditor->m_pWiringStartPin->pin()->shape()->source() ) ||
//		     (!m_pPin->shape()->source() && !m_pEditor->m_pWiringStartPin->pin()->shape()->source() ) )
//		     bGood = false;
//
//		// next the type
//		if ( bGood )
//		{
//		    if ( (m_pPin->shape()->shapeType() & m_pEditor->m_pWiringStartPin->pin()->shape()->shapeType() ) == 0 )
//			bGood = false;	// incompatible types
//		}
//
//		if ( !bGood )
//		    setCursor( *m_pEditor->m_curWireNot );
//		else
//		    setCursor( *m_pEditor->m_curWire );
//	    }
//	}
//
//    }
//    else 
//	unsetCursor();
//    QGraphicsRectItem::hoverEnterEvent( event );
//}
//
//
//void PinItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
//{
//    ATLTRACE("PinItem::hoverLeaveEvent\n");
//    unsetCursor();
//    QGraphicsRectItem::hoverLeaveEvent( event );
//}
//
//
//void PinItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
//{
//    ATLTRACE("PinItem::mousePressEvent\n");
//   if ( m_pEditor->m_eEditMode == EditMode::Wiring )
//   {
//       if ( m_pWire == NULL )
//       {
//	    ATLTRACE("PinItem::mousePressEvent accepting\n");
//	    event->accept();
//	    return;
//       }
//   }
//   event->ignore();
//}


const ShapeItem *PinItem::parentShape() const
{
    const QGraphicsItem *pParent = parentItem();

    assert( pParent != NULL );
    assert( pParent->type() == ShapeItem::Type );

    if ( pParent != NULL && pParent->type() == ShapeItem::Type )
	return 	qgraphicsitem_cast<const ShapeItem *>(pParent);

    return NULL;
}
