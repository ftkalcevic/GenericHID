#include "stdafx.h"
#include "wireitem.h"
#include "pinitem.h"
#include "shapeitem.h"

WireItem::WireItem(QPointF ptStart, QPointF ptEnd, QGraphicsItem *parent)
: QGraphicsPathItem(parent)
, m_ptStart(ptStart)
, m_ptEnd(ptEnd)
, m_pPin1( NULL )
, m_pPin2( NULL )
{
    setFlag( QGraphicsItem::ItemIsSelectable);
    MakePolygon();
}

WireItem::WireItem(PinItem *pPin1, PinItem *pPin2, QGraphicsItem *parent)
: QGraphicsPathItem(parent)
, m_pPin1( pPin1 )
, m_pPin2( pPin2 )
{
    setFlag( QGraphicsItem::ItemIsSelectable);
    MakePolygon();
    if ( m_pPin1 != NULL )
	m_pPin1->addWire( this );
    if ( m_pPin2 != NULL )
	m_pPin2->addWire( this );}

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
    //QVector<QPointF> m_Points(3);
    //m_Points[0] = m_ptStart;
    //m_Points[1] = QPointF(m_ptStart.x(),m_ptEnd.y());
    //m_Points[2] = m_ptEnd;
    //setPolygon( QPolygonF(m_Points) );

    QPainterPath path;
    path.moveTo( m_ptStart );
    //path.lineTo( QPointF(m_ptStart.x(),m_ptEnd.y()) );
    path.lineTo( m_ptEnd );
    setPath( path );
}


void WireItem::WriteXML( QDomElement &node ) const
{
    //	<Wire StartShapeId="c059aea5-bbf1-4b7b-841d-5b4f13b8dfdc" StartPinId="Output" EndShapeId="a1be9021-6352-4820-a0ac-a6a9b4c22ecf" EndPinId="PF0" />
    QDomElement wireNode = node.ownerDocument().createElement( "Wire" );
    node.appendChild( wireNode );

    XMLUtility::setAttribute( wireNode, "startShapeId", m_pPin1->parentShape()->id() );
    XMLUtility::setAttribute( wireNode, "startPinId", m_pPin1->pin()->id() );
    XMLUtility::setAttribute( wireNode, "endShapeId", m_pPin2->parentShape()->id() );
    XMLUtility::setAttribute( wireNode, "endPinId", m_pPin2->pin()->id() );
}


WireItem *WireItem::CreateFromXML( QList<ShapeItem *> &shapes, QDomElement &node )
{
    WireItem *pWire = NULL;

    int nStartShapeId = XMLUtility::getAttribute( node, "startShapeId", -1 );
    QString sStartPinId = XMLUtility::getAttribute( node, "startPinId", "" );
    int nEndShapeId = XMLUtility::getAttribute( node, "endShapeId", -1 );
    QString sEndPinId = XMLUtility::getAttribute( node, "endPinId", "" );

    // Find the actual shapes and pins
    ShapeItem *pStartShape = NULL;
    ShapeItem *pEndShape = NULL;
    PinItem *pStartPin = NULL;
    PinItem *pEndPin = NULL;

    foreach ( ShapeItem *pItem, shapes )
	if ( pItem->id() == nStartShapeId )
	{
	    pStartShape = pItem;
	    break;
	}

    foreach ( ShapeItem *pItem, shapes )
	if ( pItem->id() == nEndShapeId )
	{
	    pEndShape = pItem;
	    break;
	}

    if ( pStartShape != NULL )
    {
	foreach ( PinItem *pItem, pStartShape->pins() )
	    if ( pItem->pin()->id() == sStartPinId )
	    {
		pStartPin = pItem;
		break;
	    }
    }

    if ( pEndShape != NULL )
    {
	foreach ( PinItem *pItem, pEndShape->pins() )
	    if ( pItem->pin()->id() == sEndPinId )
	    {
		pEndPin = pItem;
		break;
	    }
    }

    if ( pStartPin != NULL && pEndPin != NULL )
    {
	pWire = new WireItem( pStartPin, pEndPin );
    }


    return pWire;
}

