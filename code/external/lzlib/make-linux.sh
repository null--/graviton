#!/bin/bash
gcc -O3 -c lzlib.c -I ../lua5.1/ -I ../zlib/
ar rcs luazlib.a lzlib.o
