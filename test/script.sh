#!/bin/sh
g++ -c test.c
g++ -o test test.o -pthread
./test
