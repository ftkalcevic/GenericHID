TEMPLATE = lib
TARGET = programmer
include(../build.pri)

CONFIG(debug,debug|release) {
    DESTDIR=debug
} else {
    DESTDIR=release
}
QT += xml
CONFIG += staticlib
DEFINES += QT_XML_LIB PROGRAMMER_LIB
CONFIG(debug,debug|release) {
    DEFINES += DEBUG
}
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/$(DESTDIR) \
    . \
    ./../include \
    ./../utility \
    ../dfuprogrammer
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/$(DESTDIR)
OBJECTS_DIR += $(DESTDIR)
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(programmer.pri)
