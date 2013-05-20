#define GVN_ACTIVATE_LOGGER

#include "graviton.hpp"
#include "core/tcp_client.hpp"
#include <iostream>
using namespace std;


int main()
{
    GraVitoN::Core::TCP_Client client("127.0.0.1", 7357);

    client.connect();
    client.send((const unsigned char *)"Hello World\n", 13);
    unsigned char *data = _null_;
    size_t ds;
    client.recv(data, ds);
    cout<<data<<endl;
    client.close();
    return 0;
}
