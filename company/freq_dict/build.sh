#!/usr/bin/env bash

BASEDIR=$(dirname $(realpath $0))
echo $BASEDIR
CURR_DIR=$PWD
cd $BASEDIR

if [ -d "build" ]; then
	rm -r build
fi
mkdir build
cd build

cmake -GNinja ..
cmake --build .
