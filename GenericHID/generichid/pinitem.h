#ifndef PINITEM_H
#define PINITEM_H

#include "editor.h"
#include "pin.h"
#include "wireitem.h"

class PinItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    PinItem( QRectF rc, const Pin *pPin, Editor *pEditor, QGraphicsItem *parent = 0 );
    ~PinItem();

    enum { Type = UserType + UserTypes::PinItemType };
    int type() const { return Type; }
    WireItem *wire() const { return m_pWire; }
    const Pin *pin() const { return m_pPin; }
    const class ShapeItem *parentShape() const;

    void setWire( WireItem *pWire ) { m_pWire = pWire; }

private:
    //virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    //virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
    //virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
    Editor *m_pEditor;
    const Pin *m_pPin;
    WireItem *m_pWire;
};

#endif // PINITEM_H
