#!/bin/sh
g++ -c proc1.cpp
g++ -o proc1 proc1.o -pthread
./proc1
