#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/tcp_client.hpp>
#include <iostream>
using namespace std;


int main()
{
    GraVitoN::Core::TCP_Client client("192.168.56.1", 7357);

    client.connect();
    client.sendString(std::string("Hello World\n"));
    cout << client.recvString() << endl;
    client.close();
    return 0;
}
