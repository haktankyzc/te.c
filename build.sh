#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra "
CURSES_FLAG="-lcurses"

gcc $CFLAGS $CURSES_FLAG -o te ./te.c
