TEMPLATE=subdirs
CONFIG += ordered
include(common.pri)
qtpropertybrowser-uselib:SUBDIRS=buildlib
QT+=widgets
SUBDIRS+=examples
DEFINES += "Q_TYPENAME=typename"
