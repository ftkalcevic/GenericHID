#!/bin/bash

svn "$@"
(cd utility; svn "$@" )
(cd usbhid; svn "$@" )
(cd qtcommon; svn "$@" )
(cd libusb-win32-device-bin-0.1.12.2; svn "$@" )
(cd libusb-1.0.12; svn "$@" )
(cd qtpropertybrowser-2.5-opensource; svn "$@" )

