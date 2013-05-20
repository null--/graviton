#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/socks5_server.hpp>
#include <iostream>
using namespace std;

int main()
{
    GraVitoN::Utils::Network_Relay_Tcp relay("127.0.0.1", 7358, "127.0.0.1", 7357, false);
    relay.run();

    // GraVitoN::Utils::SOCKS5_Server srv(7357);
    // srv.run();

	return 0;
}
