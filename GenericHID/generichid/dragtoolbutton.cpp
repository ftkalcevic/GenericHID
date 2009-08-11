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
	QPixmap shape(m_pShape->shapeFile());
	// todo get current view scale
	double dScale = 1.0;
	QPixmap scaledShape = shape.scaled( (double)shape.width() * dScale, (double)shape.height() * dScale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
	drag->setPixmap( scaledShape );

	drag->exec();
    }
}

