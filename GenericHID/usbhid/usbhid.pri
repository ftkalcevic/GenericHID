#Header files
HEADERS += hiddevice.h \
	hiddevices.h \
	hidkbdevice.h \
	hidlcddevice.h \
	hidparser.h \
	hidtypes.h

#Source files
SOURCES += hiddevice.cpp \
	hiddevices.cpp \
	hidkbdevice.cpp \
	hidlcddevice.cpp \
	hidparser.cpp \
	hidtypes.cpp

CONFIG(THREADED_HIDDEVICE) {
SOURCES += hiddevicethread.cpp 
HEADERS += hiddevicethread.h
} 

#Forms
#FORMS += 

#Resource file(s)
#RESOURCES +=

