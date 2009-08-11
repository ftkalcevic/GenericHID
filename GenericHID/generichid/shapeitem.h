#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include "editor.h"
#include "shape.h"
#include "shapecollection.h"


class ShapeItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT	    // Need Q_OBJECT to support signals and slots

public:
    ShapeItem(const Shape *pShape, int id, Editor *pEditor, QGraphicsItem *parent=NULL);
    ~ShapeItem();
    
    enum { Type = UserType + UserTypes::ShapeItemType };
    int type() const { return Type; }
    const Shape *shapeData() const { return m_pShape; }
    QList<PinItem *> pins() { return m_pins; }
    bool mirror() const { return m_bMirror; }
    void setMirror( bool b ) { m_bMirror = b; DoTransform(); }
    double rotation() const { return m_dRotate; }
    void setRotation( double d ) { m_dRotate = d; while ( m_dRotate > 360.0 ) m_dRotate -= 360.0; DoTransform(); }
    QList<PropertyValue *> &values() { return m_values; }
    int id() const { return m_nId; }

    void WriteXML( QDomElement &node ) const;
    static ShapeItem *CreateFromXML( ShapeCollection *pCol, Editor *, QDomElement & );

signals:
    void itemChange( QGraphicsItem *item, QGraphicsItem::GraphicsItemChange change, const QVariant & value );

private:
    const Shape *m_pShape;
    Editor *m_pEditor;
    double m_dRotate;
    bool m_bMirror;
    QList<PinItem *> m_pins;
    int m_nId;
    QList<PropertyValue *> m_values;

    void DoTransform();
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
    void CreateProperties();
    bool CreateGraphics();
};

#endif // SHAPEITEM_H
