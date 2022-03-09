#!/bin/sh
g++ -c proc1.c
g++ -o proc1 proc1.o -pthread
./proc1
