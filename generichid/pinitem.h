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
