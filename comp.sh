#!/bin/bash

gcc -Wall servidor.c -o servidor

gcc -Wall cliente.c -o cliente

xfce4-terminal -e ./servidor

#para ubuntu descomentar linha seguinte e comentar anterior
#gnome-terminal -e ./servidor

./cliente
