#include "stdafx.h"
#include "generichid.h"
#include <QtGui/QApplication>
#include <QtPlugin>
#include "usb.h"
#include "logcore.h"
#include "hiddevices.h"

Q_IMPORT_PLUGIN(qjpeg)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName( "GenericHID" );
    a.setOrganizationName( "FranksWorkshop" );
    a.setOrganizationDomain( "www.franksworkshop.com.au" );

    HIDDevices::Open(false);
    LogCore::SetLog( false );
    GenericHID w;
    if ( !w.initialised() )
	return 0;
    w.show();
    w.ProcessCommandline();
    return a.exec();
}
