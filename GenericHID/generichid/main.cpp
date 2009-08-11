#include "stdafx.h"
#include "generichid.h"
#include <QtGui/QApplication>
#include "usb.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName( "GenericHID" );
    a.setOrganizationName( "FranksWorkshop" );
    a.setOrganizationDomain( "www.franksworkshop.com.au" );

    usb_init();
    usb_set_debug(255);
    GenericHID w;
    w.show();
    w.ProcessCommandline();
    return a.exec();
}
