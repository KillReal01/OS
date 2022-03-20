#!/bin/sh
g++ -c proc2.cpp
g++ -o proc2 proc2.o -pthread -lrt
#sudo setcap CAP_SYS_RESOURSE=eip proc2 
sudo ./proc2
