#define GVN_ACTIVATE_LOGGER

#include <core/udp_client.hpp>
#include <iostream>

int main()
{
    GraVitoN::Core::UDP_Socket udp;

    udp.open(7358);

    cout << "sending..." <<endl;

    udp.send((const unsigned char *)"Hello World\n", 13, "127.0.0.1", 7357);

    cout << "recving..." <<endl;
    unsigned char *data = _null_;
    size_t ds;
    string ip;
    unsigned int port;
    udp.recv(data, ds, ip, port);
    cout<<"IP: " << ip << ", Port: " << port << ", Size: "<<ds<<" - "<<data<<endl;

    udp.close();

	return 0;
}
