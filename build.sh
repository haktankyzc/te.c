#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra "

gcc $CFLAGS -o te ./te.c
