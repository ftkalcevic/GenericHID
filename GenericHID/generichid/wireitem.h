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
