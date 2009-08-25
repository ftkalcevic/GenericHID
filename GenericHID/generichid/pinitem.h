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
    const QList<WireItem *> &wires() const { return m_pWires; }
    const Pin *pin() const { return m_pPin; }
    class ShapeItem *parentShape() const;

    void addWire( WireItem *pWire ) { assert(pWire); m_pWires.push_back( pWire ); }
    bool removeWire( WireItem *pWire ) { assert(pWire); return m_pWires.removeOne( pWire ); }

private:
    //virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    //virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
    //virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
    Editor *m_pEditor;
    const Pin *m_pPin;
    QList<WireItem *> m_pWires;
};

#endif // PINITEM_H
