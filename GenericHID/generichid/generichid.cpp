#include "stdafx.h"
#include "generichid.h"
#include "programdlg.h"

GenericHID::GenericHID(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
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


 */