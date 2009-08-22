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
