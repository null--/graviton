#define GVN_ACTIVATE_LOGGER

#include "graviton.hpp"
#include "core/tcp_client.hpp"
#include <iostream>
using namespace std;

class My_TCP_Client: public GraVitoN::Core::TCP_Client
{
public:
	virtual bool run()
	{
		open();
		send((const unsigned char *)"Hello World\n", 13);
		unsigned char *data = _null_;
		unsigned int ds;
		recv(data, ds);
		cout<<data<<endl;
		close();

		return true;
	}
};

int main()
{
	My_TCP_Client client;

    client.initialize("127.0.0.1", 7357);
	client.run();

    return 0;
}
