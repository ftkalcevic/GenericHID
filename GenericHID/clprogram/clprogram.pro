TEMPLATE = app
TARGET = clprogram
DESTDIR = ./debug
CONFIG += debug console
INCLUDEPATH += ./generatedfiles/debug \
    . \
    ./../include \
    ./../utility \
    ./../programmer
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/debug
OBJECTS_DIR += debug
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles
unix:LIBS += -L../utility/debug -lutility -L../dfuprogrammer/debug -ldfuprogrammer  -L../programmer/debug -lprogrammer -L../libusb-0.1.12/.libs -l:libusb.a -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -l:libusb.a

#Include file(s)
include(clprogram.pri)
