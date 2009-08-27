#include "stdafx.h"
#include "generichid.h"
#include <QtGui/QApplication>
#include "usb.h"
#include "logcore.h"
#include "hiddevices.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName( "GenericHID" );
    a.setOrganizationName( "FranksWorkshop" );
    a.setOrganizationDomain( "www.franksworkshop.com.au" );

    HIDDevices::Open(false);
    LogCore::SetLog( false );
    GenericHID w;
    w.show();
    w.ProcessCommandline();
    return a.exec();
}
