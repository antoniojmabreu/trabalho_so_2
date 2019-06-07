#!/bin/bash

#gcc -Wall -pthread servidor.c -o servidor
gcc -Wall -o servidor servidor.c -lpthread

gcc -Wall cliente.c -o cliente

xfce4-terminal -e ./servidor

#para ubuntu descomentar linha seguinte e comentar anterior
#gnome-terminal -- ./servidor

./cliente
