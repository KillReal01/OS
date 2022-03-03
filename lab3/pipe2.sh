#!/bin/sh
g++ -c pipe2.c
g++ -o pipe2 pipe2.o -pthread
./pipe2
