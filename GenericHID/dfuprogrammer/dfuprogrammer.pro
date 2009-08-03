
TEMPLATE = lib
TARGET = dfuprogrammer
DESTDIR = ./debug
QT += xml
CONFIG += staticlib
DEFINES += QT_XML_LIB PROGRAMMER_LIB
INCLUDEPATH += generatedfiles \
    generatedfiles/debug \
    . \
    ../include \
    ../utility \
    ../libusb-0.1.12
#PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/debug
OBJECTS_DIR += debug
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(dfuprogrammer.pri)
