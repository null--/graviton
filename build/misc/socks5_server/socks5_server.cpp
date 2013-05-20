#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/socks5_server.hpp>
#include <iostream>
using namespace std;

int main()
{
    GraVitoN::Utils::SOCKS5_Server srv(7357);

    srv.run();

	return 0;
}
