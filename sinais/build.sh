#!/bin/bash

mkdir out && gcc sinais.c -o ./out/sinais $(pkg-config --cflags --libs sdl3 sdl3-ttf) && ./out/sinais
