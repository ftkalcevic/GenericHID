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
        QPixmap scaledShape = shape.scaled( (int)((double)shape.width() * dScale), (int)((double)shape.height() * dScale), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        drag->setPixmap( scaledShape );

        drag->exec();
    }
}

