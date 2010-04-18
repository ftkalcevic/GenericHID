#!/bin/bash

dh_testdir || exit
dh_testroot || exit
dh_clean 
dh_installdirs

mkdir -p debian/tmp/usr/bin
mkdir -p debian/tmp/usr/share/doc/generichid
mkdir -p debian/tmp/usr/share/man/man1
mkdir -p debian/tmp/usr/share/generichid/help
mkdir -p debian/tmp/usr/share/generichid/resources

cp ../generichid/release/generichid debian/tmp/usr/bin
chrpath -d debian/tmp/usr/bin/generichid
cp ../generichid/generichid.1 debian/tmp/usr/share/man/man1/generichid.1
cp ../generichid/config.xml debian/tmp/usr/share/generichid
cp ../generichid/help/* debian/tmp/usr/share/generichid/help
cp ../generichid/resources/* debian/tmp/usr/share/generichid/resources
cp debian/copyright debian/tmp/usr/share/doc/generichid/copyright
cp ../bin/usbkey.hex debian/tmp/usr/share/generichid
cp ../bin/teensypp2.hex debian/tmp/usr/share/generichid

dh_installchangelogs

dh_strip
dh_compress
dh_fixperms
dh_installdeb
dh_makeshlibs
dh_shlibdeps
dh_gencontrol
dh_md5sums
dh_builddeb


