#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <utils/ssl_server.hpp>

using namespace std;

/// Generate DH File:
/// openssl dhparam -check -text -5 1024 -out dh1024.pem

int main()
{
    GraVitoN::Utils::SSL_Server serv(7357);
    serv.run();

}
