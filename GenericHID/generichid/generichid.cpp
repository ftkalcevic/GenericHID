#include "stdafx.h"
#include "generichid.h"
#include "programdlg.h"
#include "dragtoolbutton.h"

const char * const CONFIGDATA_FILE = "config.xml";

GenericHID::GenericHID(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
, m_pShapes( NULL )
, m_pScene( NULL )
, m_pLastSelectedShape( NULL )
{
    ui.setupUi(this);

    m_pShapes = ShapeCollection::LoadShapeCollection( CONFIGDATA_FILE );

    // Load the shapes into the tool box
    foreach ( const Shape *pShape, m_pShapes->shapes() )
    {
	DragToolButton *pButton = new DragToolButton( this, pShape );
	pButton->setIcon( QIcon(pShape->iconFile()) );
	pButton->setToolTip( pShape->description() );
	ui.toolToolBar->addWidget(  pButton );
    }

    // Create graphics scene
    m_pScene = new ShapeScene( this, -1000, -1000, 2000, 2000, this );
    ui.graphicsView->setScene( m_pScene );
    ui.graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui.graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui.graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui.graphicsView->setDragMode( QGraphicsView::RubberBandDrag );
    ui.graphicsView->setBackgroundBrush( QBrush(QColor(255,255,236)) );

    // The action group
    QActionGroup *pActionGroup = new QActionGroup(this);
    pActionGroup->addAction( ui.actionMirrorTool );
    pActionGroup->addAction( ui.actionPointerTool );
    pActionGroup->addAction( ui.actionRotateTool );
    pActionGroup->addAction( ui.actionWireLinkTool );
    ui.actionPointerTool->setChecked( true );

    // Cursors
    m_curMirror = new QCursor( QPixmap(":/GenericHID/MirrorCursor"),0,0);
    m_curMirrorOff = new QCursor( QPixmap(":/GenericHID/MirrorCursorOff"),0,0);
    m_curRotate = new QCursor( QPixmap(":/GenericHID/RotateCursor"),0,0);
    m_curRotateOff = new QCursor( QPixmap(":/GenericHID/RotateCursorOff"),0,0);
    m_curWire = new QCursor( QPixmap(":/GenericHID/WireCursor"),0,0);
    m_curWireOff = new QCursor( QPixmap(":/GenericHID/WireCursorOff"),0,0);
    m_curWireNot = new QCursor( QPixmap(":/GenericHID/WireCursorNot"),0,0);
    m_curPointer = new QCursor( QPixmap(":/GenericHID/Cursor"),0,0);
    onPointerTool();

    connect( ui.graphicsView, SIGNAL(dropShapeEvent( const ::Shape *, QPointF) ), this, SLOT(onDropShapeEvent( const ::Shape *, QPointF) ) );
    connect( m_pScene, SIGNAL(selectionChanged() ), this, SLOT(onSelectionChanged() ) );


    ui.listView->setPropertiesWithoutValueMarked(true);
    ui.listView->setRootIsDecorated(false);
    ShapeProperty::SetBrowserFactory( ui.listView );
}

GenericHID::~GenericHID()
{

}

void GenericHID::onFileOpen()
{
}

void GenericHID::onFileSave()
{
}

void GenericHID::onFileSaveAs()
{
}

void GenericHID::onFileExit()
{
}

void GenericHID::onMicrocontrollerProgram()
{
}

void GenericHID::onMicrocontrollerExport()
{
}

void GenericHID::onMicrocontrollerImportAndProgram()
{
    ProgramDlg dlg(this);
    dlg.exec();
}

void GenericHID::onDropShapeEvent( const ::Shape *pShape, QPointF pos )
{
    // Create a new shape
    QString sError;
    if ( !m_pScene->CanAdd(pShape,sError) )
    {
	QMessageBox::critical( this, "Can't add", sError );
	return;
    }

    ShapeItem *pItem = m_pScene->CreateNewShape( pShape, this, pos );

    ui.listView->clear();
    if ( pItem != NULL )
    {
	// unselect existing items, and select ours
	foreach ( QGraphicsItem *item, m_pScene->selectedItems() )
	    item->setSelected( false );
	pItem->setSelected( true );
    }
}

void GenericHID::onSelectionChanged()
{
    if ( m_pLastSelectedShape != NULL && !m_pLastSelectedShape->isSelected() )
    {
	// write back properties if this shape is no longer selected
        const ShapeProperties &pProps = m_pLastSelectedShape->shapeData()->properties();
        pProps.retreive(m_pLastSelectedShape->values());
    }

    QList<ShapeItem *> selectedShapes;
    foreach ( QGraphicsItem *pItem, m_pScene->selectedItems() )
	if ( pItem->type() == ShapeItem::Type )
	    selectedShapes.append( qgraphicsitem_cast<ShapeItem *>(pItem) );

    if ( selectedShapes.count() == 1 )
    {
        ui.listView->clear();
	m_pLastSelectedShape = selectedShapes[0];

        const ShapeProperties &pProps = m_pLastSelectedShape->shapeData()->properties();
        ShapeProperty::SetBrowserFactory( ui.listView );
        ui.listView->addProperty(pProps.topItem());
        pProps.populate(m_pLastSelectedShape->values());
    }
    else
    {
	m_pLastSelectedShape = NULL;
        ui.listView->clear();
    }
}


void GenericHID::onRotateTool()
{
    //ui.graphicsView->unsetCursor();
    SetCursor( *m_curRotateOff, *m_curRotate );
    m_eEditMode = EditMode::Rotate;
}

void GenericHID::onMirrorTool()
{
    //ui.graphicsView->unsetCursor();
    SetCursor( *m_curMirrorOff, *m_curMirror );
    m_eEditMode = EditMode::Mirror;
}

void GenericHID::onPointerTool()
{
    //ui.graphicsView->unsetCursor();
    SetCursor( *m_curPointer, *m_curPointer );
    m_eEditMode = EditMode::Pointer;
}

void GenericHID::onWireLinkTool()
{
    //ui.graphicsView->unsetCursor();
    SetCursor( *m_curWireOff, *m_curWire );
    m_eEditMode = EditMode::Wiring;
}

void GenericHID::SetCursor( QCursor &cur1, QCursor &cur2 )
{
//    ui.graphicsView->setCursor( cur1 );
//    ui.graphicsView->viewport()->setCursor( cur1 );
//    foreach( ShapeInstance *pShape, m_pShapeInstances.instances() )
//	pShape->item()->setCursor( cur2 );
}

/*
 UI => XML => EEPROM Binary => Program
 Independant Test panel

 Pins
    - use base visual class to draw and label pins

 new
  - add remove pins as we need them 
    - binary coded switch
    - LCD 4/8 bit
    - Key matrix  rows x cols
  - PWM
    - mcu attribute
	- timer 1,2,3
	- frequency shared between outputs a/b/c on a single timer
  - counter
  - watch dog
 */