TEMPLATE = app
TARGET = mytarget
include(../build.pri)
CONFIG -= qt

mytarget.commands = make -C Demos/Device/ClassDriver/Joystick
QMAKE_EXTRA_TARGETS += mytarget
PRE_TARGETDEPS += mytarget


INSTALLS += bin_files
bin_files.path = $$INSTALLDIR_RESOURCES
bin_files.files = ../bin/generic16.hex ../bin/teensypp2.hex ../bin/usbkey.hex
bin_files.CONFIG = no_check_exist

