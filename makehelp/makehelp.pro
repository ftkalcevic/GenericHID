TEMPLATE = app
TARGET = makehelp
include(../build.pri)
CONFIG(debug,debug|release) {
DESTDIR = debug
} else {
DESTDIR = release
}
CONFIG(debug,debug|release) {
    DEFINES += DEBUG
}

PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
CONFIG(debug,debug|release) {
    OBJECTS_DIR += debug
} else {
    OBJECTS_DIR += release
}

#Include file(s)
include(makehelp.pri)

