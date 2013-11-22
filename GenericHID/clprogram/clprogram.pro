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
CONFIG(debug,debug|release) {
    DEFINES += DEBUG
} 
DEPENDPATH += .
MOC_DIR += ./generatedfiles/$(DESTDIR)
OBJECTS_DIR += $(DESTDIR)
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles
CONFIG(debug,debug|release) {
    unix:LIBS += -L../utility/debug -lutility -L../dfuprogrammer/debug -ldfuprogrammer  -L../programmer/debug -lprogrammer -lusb-1.0 -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -lusb-1.0
} else {
    unix:LIBS += -L../utility/release -lutility -L../dfuprogrammer/release -ldfuprogrammer  -L../programmer/release -lprogrammer -lusb-1.0 -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -lusb-1.0
}

#Include file(s)
include(clprogram.pri)
