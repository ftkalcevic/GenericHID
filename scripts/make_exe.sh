#!/bin/bash

BIN_DIR=$1
LIB_DIR=$2

echo "#!/bin/bash"
echo ""
echo "export LD_LIBRARY_PATH=$LIB_DIR:/usr/lib/qt4/plugins/imageformats:\$LD_LIBRARY_PATH"
echo ""
echo "$BIN_DIR/generichid.bin \$*"
echo ""

return 0
