#!/bin/sh
g++ -c client.cpp
g++ -o client client.o -pthread
./client

