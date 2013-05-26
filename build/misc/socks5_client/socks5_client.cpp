#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/socks5_client.hpp>
#include <utils/socks5_server.hpp>
#include <iostream>
using namespace std;

int main()
{
    GraVitoN::Utils::SOCKS5_Client soc("127.0.0.1", 7357, "www.google.com", 80, "null", "hell");
    if( soc.connect() )
    {
        string data = "ss";
        cout<< "SENDING..." << endl;
        GraVitoN::Core::Thread::sleep(1000);
        if( soc.sendString(data) )
        {
            cout << "RECVING..." << endl;

            soc.recvString(data);
            cout << data << endl;
        }
    }
	return 0;
}
