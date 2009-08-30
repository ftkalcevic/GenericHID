#include "stdafx.h"
#include "shapescene.h"
#include "pinitem.h"
#include "shapeitem.h"
#include "shape.h"

ShapeScene::ShapeScene( Editor *pEditor, qreal x, qreal y, qreal width, qreal height, QObject * parent )
: QGraphicsScene( x, y, width, height, parent )
, m_Logger( QCoreApplication::applicationName(), "ShapeScene" )
, m_pEditor(pEditor)
, m_bMouseDown( false )
{
}

ShapeScene::~ShapeScene()
{
}



bool ShapeScene::CanAdd( const Shape *pShape, QString &sError )
{
    if ( pShape->maxInstances() > 0 )
    {
	// Make sure we don't exceed the maximum instances of this component (usually just the MCU)
	int count = 0;
	foreach ( ShapeItem *pItem, m_ShapeItems )
	{
	    if ( pItem->shapeData()->shapeType() == pShape->shapeType() )
		count++;
	}
	if ( count+1 > pShape->maxInstances() )
	{
	    sError = QString("There can only be %1 of this type of component").arg(pShape->maxInstances());
	    return false;
	}
    }
    return true;
}

// Just return maxid+1
int ShapeScene::GetUniqueShapeId()
{
    int nId = 1;
    foreach( const ShapeItem *pItem, m_ShapeItems )
	if ( pItem->id() >= nId )
	    nId = pItem->id() + 1;
    return nId;
}

ShapeItem *ShapeScene::CreateNewShape( const Shape *pShape, Editor *pEditor, QPointF pos )
{
    int nNewId = GetUniqueShapeId();
    ShapeItem *pItem = new ShapeItem( pShape, nNewId, pEditor );
    if ( pItem != NULL )
    {
	m_ShapeItems.push_back( pItem );
	pItem->setPos( pos );
	addItem( pItem );
	connect( pItem, SIGNAL(itemChange( QGraphicsItem *, QGraphicsItem::GraphicsItemChange, const QVariant & )), this, SLOT(onViewItemChanged( QGraphicsItem *, QGraphicsItem::GraphicsItemChange, const QVariant &)) );
    }
    return pItem;
}



// Currently, we only get this is an object is moved interactively by the user.  Update wire ends.
void ShapeScene::onViewItemChanged( QGraphicsItem *item, QGraphicsItem::GraphicsItemChange /*change*/, const QVariant & /*value*/)
{
    if ( item->type() == ShapeItem::Type )
    {
	ShapeItem *pItem = qgraphicsitem_cast<ShapeItem *>(item);
	UpdateWires( pItem );
	foreach(PinItem *pPin, pItem->pins())
	    foreach ( WireItem *pWire, pPin->wires() )
		pWire->UpdateEndpoints();
    }
}

void ShapeScene::RemoveWire( WireItem *pItem )
{
    // Remove wire from pins
    pItem->pin1()->removeWire( pItem );
    pItem->pin2()->removeWire( pItem );
    removeItem(pItem);		    // removeItem relinquishes ownership
    m_WireItems.removeAll(pItem);
    delete pItem;
}

void ShapeScene::RemoveShape( ShapeItem *pItem )
{
    // Remove wires from the shape's pins
    foreach (PinItem *pPin, pItem->pins() )
	foreach (WireItem *pWire, pPin->wires() )
	    RemoveWire( pWire );
    removeItem(pItem);		    // removeItem relinquishes ownership
    m_ShapeItems.removeAll( pItem );
    delete pItem;
}

void ShapeScene::UpdateWires( ShapeItem *pItem )
{
    foreach(PinItem *pPin, pItem->pins())
	foreach(WireItem *pWire, pPin->wires() )
	    pWire->UpdateEndpoints();
}

PinItem *ShapeScene::PinUnderCursor( QPointF pos )
{
    QList<QGraphicsItem *> itemsUnderCursor = items( pos );
    foreach ( QGraphicsItem *pItem, itemsUnderCursor )
    {
	if ( pItem->type() == PinItem::Type )
	    return qgraphicsitem_cast<PinItem *>(pItem);
    }
    return NULL;
}

ShapeItem *ShapeScene::ShapeUnderCursor( QPointF pos )
{
    QList<QGraphicsItem *> itemsUnderCursor = items( pos );
    foreach ( QGraphicsItem *pItem, itemsUnderCursor )
    {
	if ( pItem->type() == ShapeItem::Type )
	    return qgraphicsitem_cast<ShapeItem *>(pItem);
    }
    return NULL;
}

void ShapeScene::keyPressEvent( QKeyEvent * keyEvent )
{
    if ( keyEvent->key() == Qt::Key_Delete )
    {
	keyEvent->accept();
	QList<QGraphicsItem *> Items = selectedItems();
	// Split the lists up into wires and shape
	QList<QGraphicsItem *> Wires;
	QList<QGraphicsItem *> Shapes;
	foreach ( QGraphicsItem *pItem, Items )
	    if ( pItem->type() == WireItem::Type )
		Wires.push_back( pItem );
	    else if ( pItem->type() == ShapeItem::Type )
		Shapes.push_back( pItem );

	// Remove wires first
	foreach ( QGraphicsItem *pItem, Wires )
	    RemoveWire( qgraphicsitem_cast<WireItem *>(pItem) );

	foreach ( QGraphicsItem *pItem, Shapes ) 
	    RemoveShape( qgraphicsitem_cast<ShapeItem *>(pItem) );
    }
}

bool CanStartWire( PinItem *pPinItem, QString &sReason )
{
    if ( pPinItem == NULL )
	sReason = "Not a pin";
    else if ( !pPinItem->pin()->enabled() )
	sReason = "Pin disabled";
    else if ( pPinItem->wires().count() == 0 )
	return true;
    else if ( pPinItem->parentShape()->shapeData()->source() )
       sReason = "Can't connect multiple wires to this pin";
    else
	return true;

    return false;
}

bool CanEndWire( PinItem *pFirstPin, PinItem *pSecondPin, QString &sReason )
{
    if ( pSecondPin == NULL )
	sReason = "Not a pin";
    else if ( !pSecondPin->pin()->enabled() )
	sReason = "Disabled pin";
	 //pSecondPin->pin()->shape() != pFirstPin->pin()->shape() && // wiring to myself?
    else if ( (pSecondPin->pin()->shape()->source() && pFirstPin->pin()->shape()->source()) ||	// Source <-> sink
	      (!pSecondPin->pin()->shape()->source() && !pFirstPin->pin()->shape()->source()) )
	sReason = "Can only connect components to MCU boards";
    else if ( (pSecondPin->pin()->pinType() & pFirstPin->pin()->pinType() ) == 0 )			// next the pin type io/adc/interrupt, etc
	sReason = "Incompatible pin types";
    else 
    {
	if ( pFirstPin->wires().count() > 0 || pSecondPin->wires().count() > 0 )
	{
	    // multi wire connection.  Can only multi wire pin on !source, if source pin is shared()
	    if ( pFirstPin->wires().count() == 0 )
	    {
		if ( pSecondPin->pin()->shape()->source() )
		    sReason = "Only MCU can have multiple connections on a pin";
		else if ( !pFirstPin->pin()->shared() )
		    sReason = "This pin can't have a shared connection";
		else
		    return true;
	    }
	    else if ( pSecondPin->wires().count() == 0 )
	    {
		if ( pFirstPin->pin()->shape()->source() )
		    sReason = "Only MCU can have multiple connections on a pin";
		else if ( !pSecondPin->pin()->shared() )
		    sReason = "This pin can't have a shared connection";
		else
		    return true;
	    }
	    else
		sReason = "Too many wires";
	}
	else
	    return true;
    }
    return false;
}

void ShapeScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //ATLTRACE("ShapeScene::mousePressEvent!\n");
    bool bHandled = false;
    m_bMouseDown = true;

    switch ( m_pEditor->m_eEditMode )
    {
	case EditMode::Wiring:
	    if ( mouseEvent->button() == Qt::LeftButton )
	    {
		// Find the item we clicked on.  Must be a free pin.
		PinItem *pPinItem = PinUnderCursor( mouseEvent->scenePos() );
		QString sReason;
		if ( CanStartWire( pPinItem, sReason ) )
		{
		    m_pEditor->m_pWiringStartPin = pPinItem;
		    m_pEditor->m_pCurrentWire = new WireItem( pPinItem->mapToScene(pPinItem->boundingRect().center()), mouseEvent->scenePos() );
		    addItem( m_pEditor->m_pCurrentWire );
		    mouseEvent->accept();
		    bHandled = true;
		    //ATLTRACE("Wiring!\n");
		}
	    }
	    break;
	case EditMode::Mirror:
	    if ( mouseEvent->button() == Qt::LeftButton )
	    {
		ShapeItem *pItem = ShapeUnderCursor( mouseEvent->scenePos() );
		if ( pItem != NULL )
		{
		    pItem->setMirror( !pItem->mirror() );
		    UpdateWires( pItem );
		    mouseEvent->accept();
		    bHandled = true;
		}
	    }
	    break;
	case EditMode::Rotate:
	    if ( mouseEvent->button() == Qt::LeftButton )
	    {
		ShapeItem *pItem = ShapeUnderCursor( mouseEvent->scenePos() );
		if ( pItem != NULL )
		{
		    pItem->setRotation( pItem->rotation() + 90.0 );
		    UpdateWires( pItem );
		    mouseEvent->accept();
		    bHandled = true;
		}
	    }
	    break;
    }

    //ATLTRACE( "Handled=%d\n", bHandled );
    if ( !bHandled )
    {
	mouseEvent->ignore();
	QGraphicsScene::mousePressEvent(mouseEvent);
    }
}


void ShapeScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
 //   ATLTRACE("mouseMoveEvent!\n");

    // Set cursor

    // Find the either the shape, or pin
    QGraphicsItem *itemUnderCursor = NULL;
    QList<QGraphicsItem *> itemsUnderCursor = items( mouseEvent->scenePos() );
    foreach ( QGraphicsItem *pItem, itemsUnderCursor )
    {
	if ( pItem->type() == PinItem::Type || pItem->type() == ShapeItem::Type )
	{
	    itemUnderCursor = pItem;
	    break;
	}
    }

    switch ( m_pEditor->m_eEditMode )
    {
	case EditMode::Mirror:
	    if ( itemUnderCursor != NULL && itemUnderCursor->type() == ShapeItem::Type )
		SetCursor( *m_pEditor->m_curMirror );
	    else
		SetCursor( *m_pEditor->m_curMirrorOff );
	    break;
	case EditMode::Rotate:
	    if ( itemUnderCursor != NULL && itemUnderCursor->type() == ShapeItem::Type )
		SetCursor( *m_pEditor->m_curRotate );
	    else
		SetCursor( *m_pEditor->m_curRotateOff );
	    break;
	case EditMode::Pointer:
	    SetCursor( *m_pEditor->m_curPointer );
	    break;
	case EditMode::Wiring:
	    if ( itemUnderCursor != NULL && itemUnderCursor->type() == PinItem::Type )
	    {
		PinItem *pPinItem = qgraphicsitem_cast<PinItem *>(itemUnderCursor);
		if ( !pPinItem->pin()->enabled() )
		{
		    SetCursor( *m_pEditor->m_curWireNot );
		}
		else if ( m_pEditor->m_pWiringStartPin == NULL ) 
		{
		    // First Pin in the link
		    QString sReason;
		    if ( CanStartWire(pPinItem,sReason) )
		    {
			SetCursor( *m_pEditor->m_curWire );	 // already connected
			//ATLTRACE("Pin 1 already connected\n");
		    }
		    else
		    {
			SetCursor( *m_pEditor->m_curWireNot );
			//ATLTRACE("Pin 1 wire\n");
		    }
		}
		else
		{
		    // we are the second pin
		    QString sReason;
		    if ( CanEndWire( m_pEditor->m_pWiringStartPin, pPinItem, sReason ) )
			SetCursor( *m_pEditor->m_curWire );
		    else
			SetCursor( *m_pEditor->m_curWireNot );
		}
	    }
	    else
	    {
		SetCursor( *m_pEditor->m_curWireOff );
		//ATLTRACE("Not over a pin\n");
	    }
	    break;
    }

    // Actions
    if ( m_pEditor->m_eEditMode == EditMode::Wiring && m_pEditor->m_pCurrentWire != NULL )
    {
        m_pEditor->m_pCurrentWire->setEnd( mouseEvent->scenePos() );
	//ATLTRACE("Moving wire!\n");
    }
    else 
	mouseEvent->ignore();

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}


void ShapeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    m_bMouseDown = false;
    if ( m_pEditor->m_eEditMode == EditMode::Wiring && m_pEditor->m_pCurrentWire != NULL )
    {
	//ATLTRACE("Finished wiring!\n");
	PinItem *pSecondPin = NULL;
	QList<QGraphicsItem *> itemsUnderCursor = items( mouseEvent->scenePos() );
	foreach ( QGraphicsItem *pItem, itemsUnderCursor )
	{
	    if ( pItem->type() == PinItem::Type )
	    {
		pSecondPin = qgraphicsitem_cast<PinItem *>(pItem);
		break;
	    }
	}

	QString sReason;
	if ( CanEndWire( m_pEditor->m_pWiringStartPin, pSecondPin, sReason ) )
	{
	    m_pEditor->m_pWiringStartPin->addWire( m_pEditor->m_pCurrentWire );
	    pSecondPin->addWire( m_pEditor->m_pCurrentWire );
	    m_pEditor->m_pCurrentWire->setPin1(m_pEditor->m_pWiringStartPin);
	    m_pEditor->m_pCurrentWire->setPin2(pSecondPin);
	    m_pEditor->m_pCurrentWire->UpdateEndpoints();
	    m_WireItems.append( m_pEditor->m_pCurrentWire );

	    m_pEditor->m_pWiringStartPin->parentShape()->wireAddedEvent( m_pEditor->m_pCurrentWire );
	    pSecondPin->parentShape()->wireAddedEvent( m_pEditor->m_pCurrentWire );

	    // add to wire list
	    m_pEditor->m_pCurrentWire = NULL;
	    m_pEditor->m_pWiringStartPin = NULL;
	}
	else
	{
	    m_pEditor->m_pWiringStartPin = NULL;
	    removeItem( m_pEditor->m_pCurrentWire );
	    delete m_pEditor->m_pCurrentWire;
	    m_pEditor->m_pCurrentWire = NULL;
	}
    }
    else
	mouseEvent->ignore();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}



QString ShapeScene::makeXML()
{
    QDomDocument xml("GenericHID");

    QDomElement rootElem = xml.createElement("GenericHID");
    xml.appendChild( rootElem );

    QDomElement shapesNode = xml.createElement("Shapes");
    rootElem.appendChild( shapesNode );

    foreach ( const ShapeItem *pItem, m_ShapeItems )
    {
	pItem->WriteXML( shapesNode );
    }

    QDomElement wiresNode = xml.createElement("Wires");
    rootElem.appendChild( wiresNode );

    foreach ( const WireItem *pItem, m_WireItems )
    {
	pItem->WriteXML( wiresNode );
    }

    return xml.toString();
}

bool ShapeScene::loadXML( QDomDocument &doc, ShapeCollection *pCol )
{
    QDomElement rootElement = doc.firstChildElement( "GenericHID" );
    if ( rootElement.isNull() )
    {
	LOG_MSG( m_Logger, LogTypes::Error, "Root node is not 'GenericHID'" );
	return false;
    }

    QDomElement shapesNode = XMLUtility::firstChildElement( rootElement, "Shapes" );
    if ( shapesNode.isNull() )
    {
	LOG_MSG( m_Logger, LogTypes::Error, "Can't find 'Shapes' node" );
	return false;
    }

    QDomNodeList shapeNodes = XMLUtility::elementsByTagName( shapesNode, "Shape" );
    for ( int i = 0; i < shapeNodes.count(); i++ )
    {
	QDomElement item = shapeNodes.item(i).toElement();
	ShapeItem *pItem = ShapeItem::CreateFromXML( pCol, m_pEditor, item );
	assert( pItem != NULL );
	if ( pItem == NULL )
	{
	    LOG_MSG( m_Logger, LogTypes::Error, QString("Failed to load shape %1 on line %2").arg(item.nodeName()).arg(item.lineNumber()) );
	    return false;
	}

	m_ShapeItems.append( pItem );
	addItem( pItem );
	connect( pItem, SIGNAL(itemChange( QGraphicsItem *, QGraphicsItem::GraphicsItemChange, const QVariant & )), this, SLOT(onViewItemChanged( QGraphicsItem *, QGraphicsItem::GraphicsItemChange, const QVariant &)) );
    }

    QDomElement wiresNode = XMLUtility::firstChildElement( rootElement, "Wires" );
    if ( wiresNode.isNull() )
    {
	LOG_MSG( m_Logger, LogTypes::Error, "Can't find 'Wires' node" );
	return false;
    }

    QDomNodeList wireNodes = XMLUtility::elementsByTagName( wiresNode, "Wire" );
    for ( int i = 0; i < wireNodes.count(); i++ )
    {
	QDomElement item = wireNodes.item(i).toElement();
	WireItem *pItem = WireItem::CreateFromXML( m_ShapeItems, item );
	m_WireItems.append( pItem );
	addItem( pItem );
	pItem->UpdateEndpoints();

	pItem->pin1()->parentShape()->wireAddedEvent( pItem );
	pItem->pin2()->parentShape()->wireAddedEvent( pItem );
    }
    return true;
}


void ShapeScene::clear()
{
    QGraphicsScene::clear();	// the graphics scene takes ownership of items and deletes them
    m_WireItems.clear();
    m_ShapeItems.clear();
}


bool ShapeScene::VerifyShapes( QString &sError ) const
{
    bool bFailed = false;

    // Must have an MCU shape
    ShapeItem *pMCU = NULL;
    foreach (ShapeItem *pItem, m_ShapeItems )
	if ( pItem->shapeData()->shapeType() == ShapeType::AT90USB128 )
	{
	    pMCU = pItem;
	    break;
	}

    if ( pMCU == NULL )
    {
	bFailed = true;
	sError += "One micro-controller object must be present\n";
    }

    // Verify current consumption
    int nCurrent = 0;
    foreach (ShapeItem *pItem, m_ShapeItems )
	nCurrent += pItem->current();

    if ( nCurrent > 500 )
    {
	bFailed = true;
	sError += QString("Device current exceeds 500mA (%1)\n").arg(nCurrent);
    }

    // Verify individual shapes
    foreach (ShapeItem *pItem, m_ShapeItems )
	bFailed = !pItem->Verify( sError ) || bFailed;

    return !bFailed;
}


QString ShapeScene::MakeDeviceXML( ) const
{
    QDomDocument xml("GenericHIDDevice");

    QDomElement rootElem = xml.createElement("GenericHIDDevice");
    xml.appendChild( rootElem );

    // Find the MCU and sum the current
    ShapeItem *pMCU = NULL;
    int nCurrent = 0;
    foreach (ShapeItem *pItem, m_ShapeItems )
    {
	if ( pItem->shapeData()->shapeType() == ShapeType::AT90USB128 )
	    pMCU = pItem;
	nCurrent += pItem->current();
    }

    // Find the optional Power Pin
    QString sPowerPin;
    foreach (ShapeItem *pItem, m_ShapeItems )
	if ( pItem->shapeData()->shapeType() == ShapeType::Power )
	{
	    sPowerPin = pItem->shapeData()->GetPort( pItem->pins(), "G" );
	    break;
	}
    
    if ( pMCU != NULL )
    {
	// Output MCU first.
	pMCU->MakeDeviceXML( rootElem, nCurrent, sPowerPin );

	QDomElement controls = xml.createElement("Controls");
	rootElem.appendChild( controls );

	// And the shapes/controls
	foreach (ShapeItem *pItem, m_ShapeItems )
	    pItem->MakeControlsXML( controls );
    }

    return xml.toString();
}
