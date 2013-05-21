#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/socks5_server.hpp>
#include <iostream>
using namespace std;

int main()
{
    cout << GraVitoN::Utils::Netkit::dnsLookup4("www.google.com") << endl;
    cout << GraVitoN::Utils::Netkit::dnsLookup4("mail.google.com") << endl;

    // GraVitoN::Utils::Relay_Server_Tcp relay("127.0.0.1", 7357, "127.0.0.1", 80, false);
    // relay.run();

    GraVitoN::Utils::SOCKS5_Server srv(7357);
    // GraVitoN::Utils::SOCKS5_Server srv(7357, "null", "hell");
    srv.run();

	return 0;
}
