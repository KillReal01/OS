#!/bin/sh
g++ -c mut2.c
g++ -o mut2 mut2.o -pthread
./mut2
