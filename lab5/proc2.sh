#!/bin/sh
g++ -c proc2.cpp
g++ -o proc2 proc2.o -pthread
./proc2
