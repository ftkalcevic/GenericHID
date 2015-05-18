This directory is a bit of a fudge.  

The old version of qmake doesn allow the installation of extra executable 
files that dont exist before the build.  They are copied correctly but are 
treated as plain files and lose their executable bits.

Newer versions of qmake support... 

myinstalltarget.CONFIG = no_check_exist executable


