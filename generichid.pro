TEMPLATE = subdirs
SUBDIRS += utility/utility.pro \
    qtpropertybrowser-2.5-opensource/buildlib/buildlib.pro \
    qtcommon/qtcommon.pro \
    dfuprogrammer/dfuprogrammer.pro \
    programmer/programmer.pro \
    clprogram/clprogram.pro \
    makeeeprom/makeeeprom.pro \
    usbhid/usbhid.pro \
    generichid/generichid.pro \
    "LUFA_100219/firmware.pro" \
    scripts/scripts.pro
CONFIG += ordered
