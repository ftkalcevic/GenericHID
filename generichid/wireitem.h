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

#ifndef WIREITEM_H
#define WIREITEM_H

class PinItem;
class ShapeItem;

class WireItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    WireItem(QPointF ptStart, QPointF ptEnd, QGraphicsItem *parent=NULL);
    WireItem(PinItem *pPin1, PinItem *pPin2, QGraphicsItem *parent=NULL);
    ~WireItem();

    void setStart(QPointF pt);
    void setEnd(QPointF pt);

    enum { Type = UserType + UserTypes::WireItemType };
    int type() const { return Type; }
    PinItem *pin1() const { return m_pPin1; }
    PinItem *pin2() const { return m_pPin2; }
    void setPin1( PinItem *pPin ) { m_pPin1 = pPin; }
    void setPin2( PinItem *pPin ) { m_pPin2 = pPin; }
    void UpdateEndpoints();
    void WriteXML( QDomElement &node ) const;
    static WireItem *CreateFromXML( QList<ShapeItem *> &shapes, QDomElement &node, QString &sError );

private:
    void MakePolygon();
    virtual QPainterPath shape() const;
    virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    virtual QRectF boundingRect() const;

    QPointF m_ptStart;
    QPointF m_ptEnd;
    PinItem *m_pPin1;
    PinItem *m_pPin2;
    QPainterPath m_shapePath;
    QRectF m_rcBounds;
};

#endif // WIREITEM_H
