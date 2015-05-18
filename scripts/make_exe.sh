#!/bin/bash

BIN_DIR=$1
LIB_DIR=$2
QT_PLUGINS_DIR=$(QT_SELECT=4 qmake -query QT_INSTALL_PLUGINS)

echo "#!/bin/bash"
echo ""
echo "export LD_LIBRARY_PATH=$LIB_DIR:$QT_PLUGINS_DIR/imageformats:\$LD_LIBRARY_PATH"
echo ""
echo "$BIN_DIR/generichid.bin \$*"
echo ""

return 0
