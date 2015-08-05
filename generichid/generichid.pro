TEMPLATE = app
TARGET = generichid.bin
include(../build.pri)
CONFIG(debug,debug|release) {
DESTDIR = debug
} else {
DESTDIR = release
}
QT += xml svg
QTPLUGIN += qjpeg
CONFIG += precompile_header link_prl
DEFINES += QT_XML_LIB QT_SVG_LIB USING_PCH
CONFIG(debug,debug|release) {
    DEFINES += DEBUG
}
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/$(DESTDIR) \
    ../include \
    ../utility \
    ../programmer \
    ../dfuprogrammer \
    /usr/include/libusb-1.0 \
    ../qtpropertybrowser-2.5-opensource/src \
    ../makeeeprom \
    . \
    ../usbhid \
    ../qtcommon

CONFIG(debug,debug|release) {
INCLUDEPATH += ./generatedfiles/debug
} else {
INCLUDEPATH += ./generatedfiles/release
}

CONFIG(debug,debug|release) {
    unix:LIBS += -L../qtcommon/debug -lqtcommon -L../usbhid/debug -lusbhid -L../makeeeprom/debug -lmakeeeprom -L../utility/debug -lutility -L../dfuprogrammer/debug -ldfuprogrammer  -L../programmer/debug -lprogrammer -lusb-1.0 -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -lusb-1.0 -L"../qtpropertybrowser-2.5-opensource/lib" -lQtSolutions_PropertyBrowser-2.5 -lutility -lutility -lutility -L$(QTDIR)/plugins/imageformats -lqjpeg
} else {
    unix:LIBS += -L../qtcommon/release -lqtcommon -L../usbhid/release -lusbhid -L../makeeeprom/release -lmakeeeprom -L../utility/release -lutility -L../dfuprogrammer/release -ldfuprogrammer  -L../programmer/release -lprogrammer -lusb-1.0 -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -lusb-1.0 -L"../qtpropertybrowser-2.5-opensource/lib" -lQtSolutions_PropertyBrowser-2.5 -lutility -lutility -lutility -L$(QTDIR)/plugins/imageformats -lqjpeg -lmakeeeprom
}
CONFIG(debug,debug|release) {
    PRE_TARGETDEPS = ../dfuprogrammer/debug/libdfuprogrammer.a ../makeeeprom/debug/libmakeeeprom.a ../programmer/debug/libprogrammer.a ../qtcommon/debug/libqtcommon.a ../utility/debug/libutility.a ../usbhid/debug/libusbhid.a
} else {
    PRE_TARGETDEPS = ../dfuprogrammer/release/libdfuprogrammer.a ../makeeeprom/release/libmakeeeprom.a ../programmer/release/libprogrammer.a ../qtcommon/release/libqtcommon.a ../utility/release/libutility.a ../usbhid/release/libusbhid.a
}

PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
CONFIG(debug,debug|release) {
    MOC_DIR += ./generatedfiles/debug
    OBJECTS_DIR += debug
} else {
    MOC_DIR += ./generatedfiles/release
    OBJECTS_DIR += release
}

UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

target.path = $$INSTALLDIR_BIN
help_files.files = help config.xml resources 
help_files.path = $$INSTALLDIR_RESOURCES
man_files.files = generichid.1
man_files.path = $$INSTALLDIR_RESOURCES/man/man1
INSTALLS += target help_files man_files 

#Include file(s)
include(generichid.pri)

