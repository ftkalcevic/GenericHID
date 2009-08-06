#include "stdafx.h"
#include "dragtoolbutton.h"
#include "shapemimedata.h"

DragToolButton::DragToolButton(QWidget *parent, const Shape *pShape )
: QToolButton(parent)
, m_pShape( pShape )
{

}

DragToolButton::~DragToolButton()
{

}


void DragToolButton::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )//&& iconLabel->geometry().contains(event->pos()) ) 
    {
	QDrag *drag = new QDrag(this);
	ShapeMimeData *mimeData = new ShapeMimeData( m_pShape );

	drag->setMimeData(mimeData);
	drag->setPixmap(QPixmap(m_pShape->shapeFile()));

	Qt::DropAction dropAction = drag->exec();
    }
}

