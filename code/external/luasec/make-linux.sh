#!/bin/bash
echo "--------------------------------------------------------"
gcc -c *.c -I ../lua5.1/ ../openssl/include/openssl/

echo "--------------------------------------------------------"
ar rcs luasocket.a *.o
