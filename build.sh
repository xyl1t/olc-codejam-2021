#!/bin/bash
set -e

#mac
if [[ "$OSTYPE" == "darwin"* ]]; then
	echo "Building for mac"
	make -f Makefile_mac $1 
else
	echo "Building for Linux"
	make -f Makefile_linux $1
fi
