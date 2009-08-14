TEMPLATE = lib
TARGET = makeeeprom
DESTDIR = ../debug
QT += xml
CONFIG += debug
DEFINES += QT_XML_LIB MAKEEEPROM_LIB
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/debug \
    .
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/debug
OBJECTS_DIR += debug
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(makeeeprom.pri)
