#!/bin/bash

rm a
gcc main.c -o a `sdl2-config --cflags --libs`
./a