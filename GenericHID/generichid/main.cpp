// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include "generichid.h"
#include <QtGui/QApplication>
#include <QtPlugin>
#include <QDir>
#include "logcore.h"
#include "hiddevices.h"

//Q_IMPORT_PLUGIN(qjpeg)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName( "GenericHID" );
    a.setOrganizationName( "FranksWorkshop" );
    a.setOrganizationDomain( "www.franksworkshop.com.au" );

#ifdef DEBUG
    QDir::addSearchPath("config","./");
    QDir::addSearchPath("resources","./resources/");
#else
    #ifdef __linux
        QDir::addSearchPath("config", "/usr/share/generichid");
        QDir::addSearchPath("resources", "/usr/share/generichid/resources");
    #else
        QDir::addSearchPath("config", QCoreApplication::applicationDirPath() );
        QDir::addSearchPath("resources", QCoreApplication::applicationDirPath() + "/resources");
    #endif
#endif

    HIDDevices::Open(0);
    LogCore::SetLog( false );
    GenericHID w;
    if ( !w.initialised() )
	return 0;
    w.show();
    w.ProcessCommandline();
    return a.exec();
}
