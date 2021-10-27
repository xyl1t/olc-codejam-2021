#!/bin/bash
set -e

#mac
if [[ "$OSTYPE" == "darwin"* ]]; then
	echo "Building for mac"
	make -f Makefile_mac
else
	echo "Building for Linux"
	make debug -f Makefile_linux
fi
