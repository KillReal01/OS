#!/bin/sh
g++ -c pipe.c
g++ -o pipe pipe.o -pthread
./pipe
