TEMPLATE = lib
TARGET = makeeeprom
CONFIG += staticlib qt
include(../build.pro)

CONFIG(debug,debug|release) {
DESTDIR=debug
} else {
DESTDIR=release
}

QT += xml

DEFINES += QT_XML_LIB
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/debug \
    ../include \
    ../utility \
    .
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/debug
OBJECTS_DIR += debug
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(makeeeprom.pri)
