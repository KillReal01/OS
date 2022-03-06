#!/bin/sh
g++ prog1.c -o prog1
g++ prog2.c -w -o prog
./prog "$@"
