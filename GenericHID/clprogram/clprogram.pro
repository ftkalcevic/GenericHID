TEMPLATE = app
TARGET = clprogram
CONFIG(debug,debug|release) {
    DESTDIR=debug
} else {
    DESTDIR=release
}
CONFIG += console
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/$(DESTDIR) \
    . \
    ./../include \
    ./../utility \
    ./../programmer
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./generatedfiles/$(DESTDIR)
OBJECTS_DIR += $(DESTDIR)
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles
CONFIG(debug,debug|release) {
    unix:LIBS += -L../utility/debug -lutility -L../dfuprogrammer/debug -ldfuprogrammer  -L../programmer/debug -lprogrammer -L../libusb-0.1.12/.libs -l:libusb.a -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -l:libusb.a
} else {
    unix:LIBS += -L../utility/release -lutility -L../dfuprogrammer/release -ldfuprogrammer  -L../programmer/release -lprogrammer -L../libusb-0.1.12/.libs -l:libusb.a -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -l:libusb.a
}

#Include file(s)
include(clprogram.pri)
