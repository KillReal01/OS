#!/bin/sh
g++ -c fcntl.c
g++ -o fcntl fcntl.o -pthread
./fcntl
