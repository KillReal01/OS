#!/bin/sh
g++ -c proc2.c
g++ -o proc2 proc2.o -pthread
./proc2
