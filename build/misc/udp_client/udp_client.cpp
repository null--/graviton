#define GVN_ACTIVATE_LOGGER

#include "core/thread.hpp"
#include "core/udp_client.hpp"
#include <iostream>

int main()
{
    GraVitoN::Core::UDP_Client udp("127.0.0.1", 7357);

    udp.open();

    cout << "sending..." <<endl;

    udp.send((const unsigned char *)"Hello World\n", 13);

    cout << "recving..." <<endl;
    unsigned char *data = _null_;
    size_t ds;
    udp.recv(data, ds);
    cout<<"Size: "<<ds<<" - "<<data<<endl;

    udp.close();

	return 0;
}
