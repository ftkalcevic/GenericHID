#include "stdafx.h"
#include "generichid.h"
#include "programdlg.h"
#include "dragtoolbutton.h"

const char * const CONFIGDATA_FILE = "config.xml";

GenericHID::GenericHID(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
, m_pShapes( NULL )
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