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
#include "shapegraphicsview.h"
#include "shapemimedata.h"

ShapeGraphicsView::ShapeGraphicsView(QWidget *parent)
: QGraphicsView(parent)
, m_dViewScale( 1.0 )
{
    setAcceptDrops( true );
}

ShapeGraphicsView::~ShapeGraphicsView()
{

}


void ShapeGraphicsView::dropEvent ( QDropEvent * event )
{
    const ShapeMimeData *mimeData = qobject_cast<const ShapeMimeData *>(event->mimeData());
    if ( mimeData != NULL )
    {
	emit dropShapeEvent( mimeData->shape(), mapToScene(event->pos()) );
	event->acceptProposedAction();
    }
    //else
	//QGraphicsView::dropEvent( event );
}

void ShapeGraphicsView::dragEnterEvent( QDragEnterEvent * event )
{
    const ShapeMimeData *mimeData = qobject_cast<const ShapeMimeData *>(event->mimeData());
    if ( mimeData != NULL )
	event->acceptProposedAction();
    //else
	//QGraphicsView::dragEnterEvent( event );
}

void ShapeGraphicsView::dragMoveEvent( QDragMoveEvent * event )
{
    const ShapeMimeData *mimeData = qobject_cast<const ShapeMimeData *>(event->mimeData());
    if ( mimeData != NULL )
	event->acceptProposedAction();
    //else
	//QGraphicsView::dragMoveEvent( event );
}


void ShapeGraphicsView::wheelEvent(QWheelEvent *event)
{
    int nDelta = event->delta();
    double dScale = pow((double)2, nDelta / 240.0);
    //ATLTRACE( "nDelta=%d, dScale=%f\n", nDelta, dScale );
    scaleView( m_dViewScale * dScale );
}


void ShapeGraphicsView::scaleView( double dScale )
{
    m_dViewScale = dScale;
    if ( m_dViewScale < 0.1 )
	m_dViewScale = 0.1;
    else if ( m_dViewScale > 5 )
	m_dViewScale = 5;

    QMatrix m;
    m.scale( m_dViewScale,m_dViewScale );
    setMatrix( m );
    emit sceneScaleChanged( m_dViewScale );
}
