#include "stdafx.h"
#include "generichid.h"
#include "programdlg.h"
#include "dragtoolbutton.h"

const char * const CONFIGDATA_FILE = "config.xml";

GenericHID::GenericHID(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
, m_pShapes( NULL )
, m_pScene( NULL )
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
    m_pScene = new QGraphicsScene( -1000, -1000, 2000, 2000, this );
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
    m_curRotate = new QCursor( QPixmap(":/GenericHID/RotateCursor"),0,0);
    m_curWire = new QCursor( QPixmap(":/GenericHID/WireCursor"),0,0);
    m_curPointer = new QCursor( QPixmap(":/GenericHID/Cursor"),0,0);
    onPointerTool();

    connect( ui.graphicsView, SIGNAL(dropShapeEvent( const ::Shape *, QPointF) ), this, SLOT(onDropShapeEvent( const ::Shape *, QPointF) ) );
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
    if ( !m_pShapeInstances.CanAdd(pShape,sError) )
    {
	QMessageBox::critical( this, "Can't add", sError );
	return;
    }

    ShapeInstance *pInstance = m_pShapeInstances.CreateNewShape( pShape );
    if ( pInstance != NULL )
    {
	pInstance->item()->setPos( pos );
	m_pScene->addItem( pInstance->item() );
    }
}

void GenericHID::onRotateTool()
{
    ui.graphicsView->setCursor( *m_curRotate );
}

void GenericHID::onMirrorTool()
{
    ui.graphicsView->setCursor( *m_curMirror );
}
void GenericHID::onPointerTool()
{
    ui.graphicsView->setCursor( *m_curPointer );//ui.graphicsView->setCursor( QCursor(Qt::ArrowCursor) );
}
void GenericHID::onWireLinkTool()
{
    ui.graphicsView->setCursor( *m_curWire );
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