#!/bin/sh
g++ prog.c -w -o prog
sudo setcap CAP_SYS_ADMIN=eip prog
./prog
