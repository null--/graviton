#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/udp_socket.hpp>
#include <iostream>
using namespace std;


int main()
{
    GraVitoN::Core::UDP_Socket client;

    // client.open();
    // client.sendString( GraVitoN::Core::UDP_Packet_String("Hello World\n", "127.0.0.1", 7357) );
    // cout << client.recvString().data << endl;
    // client.close();

    client.open();
    client.bind("127.0.0.1", 7358);
    cout << client.recvString().data << endl;
    client.close();
    
    return 0;
}
