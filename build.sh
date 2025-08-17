#!/bin/sh
# TODO: fix 'sh' location (i'm on mac)

set -xe

cc -O0 -Wall -Wextra -o tup0 main.c

time ./tup0
