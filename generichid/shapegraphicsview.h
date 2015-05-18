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

#ifndef SHAPEGRAPHICSVIEW_H
#define SHAPEGRAPHICSVIEW_H

#include "shape.h"

class ShapeGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    ShapeGraphicsView(QWidget *parent);
    ~ShapeGraphicsView();

    void scaleView( double dScale );
    double scale() const { return m_dViewScale; }

signals:
    void dropShapeEvent( const ::Shape *pShape, QPointF pos );
    void sceneScaleChanged( double d );

private:
    virtual void dropEvent( QDropEvent * event );
    virtual void dragEnterEvent( QDragEnterEvent * event );	
    virtual void dragMoveEvent( QDragMoveEvent * event );
    virtual void wheelEvent(QWheelEvent *event);
    double m_dViewScale;
};

#endif // SHAPEGRAPHICSVIEW_H
