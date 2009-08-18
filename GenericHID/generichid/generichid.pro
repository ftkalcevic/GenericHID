TEMPLATE = app
TARGET = GenericHID
DESTDIR = ./debug
QT += xml svg xmlpatterns
CONFIG += debug precompile_header
DEFINES += QT_XML_LIB QT_SVG_LIB USING_PCH
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/debug \
    ../include \
    ../utility \
    ../programmer \
    ../dfuprogrammer \
    ../libusb-0.1.12 \
    ../qtpropertybrowser-2.5-opensource/src \
    ../makeeeprom \
    .
unix:LIBS += -L../makeeeprom/debug -lmakeeeprom -L../utility/debug -lutility -L../dfuprogrammer/debug -ldfuprogrammer  -L../programmer/debug -lprogrammer -L../libusb-0.1.12/.libs -l:libusb.a -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -l:libusb.a -L"../qtpropertybrowser-2.5-opensource/lib" -l:libQtSolutions_PropertyBrowser-2.5.a -lutility -lutility -lutility
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/debug
OBJECTS_DIR += debug
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(generichid.pri)

