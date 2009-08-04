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
    ../libusb-0.1.12 \
    .
unix:LIBS += -L../utility/debug -lutility -L../dfuprogrammer/debug -ldfuprogrammer  -L../programmer/debug -lprogrammer -L../libusb-0.1.12/.libs -l:libusb.a -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -l:libusb.a
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/debug
OBJECTS_DIR += debug
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(generichid.pri)

