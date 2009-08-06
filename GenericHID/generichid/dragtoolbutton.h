#ifndef DRAGTOOLBUTTON_H
#define DRAGTOOLBUTTON_H

#include "shape.h"

class DragToolButton : public QToolButton
{
    Q_OBJECT

public:
    DragToolButton(QWidget *parent, const Shape *pShape);
    ~DragToolButton();

private:
    virtual void mousePressEvent(QMouseEvent *event);
    const Shape *m_pShape;
};

#endif // DRAGTOOLBUTTON_H
