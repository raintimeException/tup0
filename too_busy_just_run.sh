#!/bin/sh

set -e

cc -o nob nob.c

./nob

echo "An executable is located in the 'build/' folder :)"

./build/tup0
