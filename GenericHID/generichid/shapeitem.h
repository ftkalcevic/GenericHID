#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include "editor.h"
#include "shape.h"


class ShapeItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT	    // Need Q_OBJECT to support signals and slots

public:
    ShapeItem(const Shape *pShape, Editor *pEditor, QGraphicsItem *parent=NULL);
    ~ShapeItem();
    
    enum { Type = UserType + UserTypes::ShapeItemType };
    int type() const { return Type; }
    bool CreateGraphics();
    const Shape *shapeData() const { return m_pShape; }
    QList<PinItem *> pins() { return m_pins; }

signals:
    void itemChange( QGraphicsItem *item, QGraphicsItem::GraphicsItemChange change, const QVariant & value );

private:
    const Shape *m_pShape;
    Editor *m_pEditor;
    double m_dRotate;
    bool m_bMirror;
    QList<PinItem *> m_pins;

    void DoTransform();
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
};

#endif // SHAPEITEM_H
