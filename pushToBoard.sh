#!/usr/bin/env bash

if [ $# -ne 1 ]; then
	echo "Usage: ./pushToBoard.sh <board ip>"
	exit
fi

cp -r ./hpc_utils/bin/* ./to_push/
cp -r ./hpc_utils/tests/bin/* ./to_push/
cp -r ./hpc_utils_lib/*.* ./to_push/
cp -r ./hpc_utils_lib/tests/bin/* ./to_push/
scp -r ./to_push/* root@$1:/root/
