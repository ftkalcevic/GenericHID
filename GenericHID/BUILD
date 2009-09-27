GenericHID GUI
==============

Linux
-----
GenericHID GUI required Qt 4.5.2.  The release version is built against the
static library version.  I build this using the configure command...

./configure -opensource -static -no-rpath -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -no-sql-db2 -no-sql-ibase -no-sql-mysql -no-sql-oci -no-sql-odbc -no-sql-psql -no-sql-sqlite -no-sql-sqlite2 -no-sql-tds -nomake examples -nomake demos -nomake docs -nomake translations -prefix /home/frankt/qt/qtsdk-2009.03/qt 

Building
========

> cd libusb-0.1.12
> ./configure
> make
> cd ..
> qmake
> make

The binaries are stored in generichid/release and generichid/debug


Win32
-----

I use Visual Studio 2005 to build GenericHID.  Use the generichid.sln solution 
file.  The release version is built against the static libraries of Qt4.5.2. 
I build qt using the following settings...

configure -debug-and-release -opensource -static -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg

To use the C runtime static libary, the Qt build needs to use the /MT build 
option.  I edited <QTDir>/mkspecs/win32-msvc2005/qmake.conf and changed

QMAKE_CFLAGS_RELEASE    = -O2 -MT



GenericHID firmware
===================

The makefile for the firmware is in MyUSB/Demos/Joystick.  I have only built
the firmware using the Win32 Winavr package, but everything is standard and
should build on linux.

