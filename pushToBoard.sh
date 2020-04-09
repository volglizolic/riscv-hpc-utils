#!/usr/bin/env bash

if [ $# -ne 1 ]; then
	echo "Usage: ./pushToBoard.sh <board ip>"
	exit
fi

scp -r ./bin/* root@$1:/root/
