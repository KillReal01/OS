#!/bin/sh
g++ -c pipe.c
g++ -o lab pipe.o -pthread
./lab
