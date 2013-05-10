#/bin/bash

g++ -c *.cpp mt/*.cpp net/*.cpp fs/*.cpp
ar rcs libting.a *.o
