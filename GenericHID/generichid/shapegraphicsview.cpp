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
