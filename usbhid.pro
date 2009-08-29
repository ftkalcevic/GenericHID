TEMPLATE = lib
TARGET = usbhid
CONFIG += staticlib qt create_prl
include(../build.pro)

CONFIG(debug,debug|release) {
DESTDIR=debug
} else {
DESTDIR=release
}

QT += xml xmlpatterns

DEFINES += QT_XML_LIB

INCLUDEPATH +=  ./generatedfiles
CONFIG(debug,debug|release) {
INCLUDEPATH +=  ./generatedfiles/debug
} else {
INCLUDEPATH +=  ./generatedfiles/release
}

win32:INCLUDEPATH += ../libusb-win32-device-bin-0.1.12.1/include
unix:INCLUDEPATH += ../utility ../usbhid ../hid ../include
CONFIG(USBLIB01) {
unix:INCLUDEPATH += ../libusb-0.1.12
} else {
unix:INCLUDEPATH += ../libusb-1.0.2/libusb
}
    
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
include(usbhid.pri) 

