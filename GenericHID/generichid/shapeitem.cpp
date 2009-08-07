#include "stdafx.h"
#include "shapeitem.h"

ShapeItem::ShapeItem(const QString &fileName, Editor *pEditor, QGraphicsItem *parent)
: QGraphicsPixmapItem(QPixmap(fileName),parent)
, m_pEditor( pEditor )
{
    setAcceptHoverEvents( true );
    setToolTip( "I have a tool tip" );
}

ShapeItem::~ShapeItem()
{

}

void ShapeItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
    if ( m_pEditor->m_eEditMode == EditMode::Mirror )
	setCursor( *(m_pEditor->m_curMirror) );
    else if ( m_pEditor->m_eEditMode == EditMode::Rotate )
	setCursor( *(m_pEditor->m_curRotate) );
}