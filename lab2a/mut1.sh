#!/bin/sh
g++ -c mut1.c
g++ -o mut1 mut1.o -pthread
./mut1
