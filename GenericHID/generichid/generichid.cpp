#include "stdafx.h"
#include "generichid.h"

GenericHID::GenericHID(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

GenericHID::~GenericHID()
{

}


/*
 UI => XML => EEPROM Binary => Program
 Independant Test panel

 Pins
    - use base visual class to draw and label pins


 */