#!/bin/sh
g++ -c proc1.cpp
g++ -o proc1 proc1.o -pthread -lrt
sudo setcap CAP_SYS_RESOURSE=eip proc1 
./proc1

