#!/bin/sh
g++ -c server.cpp
g++ -o server server.o -pthread
./server
