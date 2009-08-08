#include "stdafx.h"
#include "wireitem.h"
#include "pinitem.h"

WireItem::WireItem(QPointF ptStart, QPointF ptEnd, QGraphicsItem *parent)
: QGraphicsPolygonItem(parent)
, m_ptStart(ptStart)
, m_ptEnd(ptEnd)
, m_pPin1( NULL )
, m_pPin2( NULL )
{
    MakePolygon();
}

WireItem::~WireItem()
{
}

void WireItem::setStart(QPointF pt)
{
    m_ptStart = pt;
    MakePolygon();
}

void WireItem::setEnd(QPointF pt)
{
    m_ptEnd = pt;
    MakePolygon();
}

void WireItem::UpdateEndpoints()
{
    m_ptStart = m_pPin1->mapToScene(m_pPin1->boundingRect().center());
    m_ptEnd = m_pPin2->mapToScene(m_pPin2->boundingRect().center());
    MakePolygon();
}


void WireItem::MakePolygon()
{
    QVector<QPointF> m_Points(2);
    m_Points[0] = m_ptStart;
    m_Points[1] = m_ptEnd;
    setPolygon( QPolygonF(m_Points) );
}
