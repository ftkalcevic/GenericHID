
TEMPLATE = lib
TARGET = programmer
DESTDIR = ./debug
QT += xml
CONFIG += staticlib
DEFINES += QT_XML_LIB PROGRAMMER_LIB
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/debug \
    . \
    ./../include \
    ./../utility
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/Debug
OBJECTS_DIR += debug
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(programmer.pri)
