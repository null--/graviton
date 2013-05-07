#!/bin/bash
echo "--------------------------------------------------------"
gcc -c context.c -I ../lua5.1/ ../openssl/include/openssl/
gcc -c ssl.c -I ../lua5.1/ ../openssl/include/openssl/

echo "--------------------------------------------------------"
ar rcs luasec.a *.o
