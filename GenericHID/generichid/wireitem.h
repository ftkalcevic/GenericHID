#ifndef WIREITEM_H
#define WIREITEM_H

class PinItem;
class ShapeItem;

class WireItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    WireItem(QPointF ptStart, QPointF ptEnd, QGraphicsItem *parent=NULL);
    WireItem(PinItem *pPin1, PinItem *pPin2, QGraphicsItem *parent=NULL);
    ~WireItem();

    void setStart(QPointF pt);
    void setEnd(QPointF pt);

    PinItem *pin1() const { return m_pPin1; }
    PinItem *pin2() const { return m_pPin2; }
    void setPin1( PinItem *pPin ) { m_pPin1 = pPin; }
    void setPin2( PinItem *pPin ) { m_pPin2 = pPin; }
    void UpdateEndpoints();
    void WriteXML( QDomElement &node ) const;
    static WireItem *CreateFromXML( QList<ShapeItem *> &shapes, QDomElement &node );

private:
    void MakePolygon();

    QPointF m_ptStart;
    QPointF m_ptEnd;
    PinItem *m_pPin1;
    PinItem *m_pPin2;
};

#endif // WIREITEM_H
