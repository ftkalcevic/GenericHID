TEMPLATE = lib
TARGET = qtcommon
CONFIG += staticlib qt
include(../build.pri)

CONFIG(debug,debug|release) {
DESTDIR=debug
} else {
DESTDIR=release
}

QT += xml

DEFINES += QT_XML_LIB
CONFIG(debug,debug|release) {
    DEFINES += DEBUG
}

INCLUDEPATH +=  ./generatedfiles ../utility
CONFIG(debug,debug|release) {
INCLUDEPATH +=  ./generatedfiles/debug
} else {
INCLUDEPATH +=  ./generatedfiles/release
}

win32:INCLUDEPATH +=
unix:INCLUDEPATH += ../qtcommon ../usbhid ../hid ../include
    
DEPENDPATH += .
CONFIG(debug,debug|release) {
MOC_DIR += ./generatedfiles/debug
} else {
MOC_DIR += ./generatedfiles/release
}
CONFIG(debug,debug|release) {
OBJECTS_DIR += debug
} else {
OBJECTS_DIR += release
}
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(qtcommon.pri) 

