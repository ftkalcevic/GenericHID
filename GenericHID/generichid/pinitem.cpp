// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include "pinitem.h"
#include "shape.h"
#include "shapeitem.h"

PinItem::PinItem( QRectF rc, const Pin *pPin, Editor *pEditor, QGraphicsItem *parent )
: QGraphicsRectItem( rc, parent )
, m_pEditor( pEditor )
, m_pPin( pPin )
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


ShapeItem *PinItem::parentShape() const
{
    QGraphicsItem *pParent = parentItem();

    assert( pParent != NULL );
    assert( pParent->type() == ShapeItem::Type );

    if ( pParent != NULL && pParent->type() == ShapeItem::Type )
	return 	qgraphicsitem_cast<ShapeItem *>(pParent);

    return NULL;
}
