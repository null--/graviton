#define GVN_ACTIVATE_LOGGER

#include "graviton.hpp"
#include "core/tcp_server.hpp"
#include <iostream>
using namespace std;

class My_Server : public GraVitoN::Core::TCP_Server
{
public:
    virtual bool response(GraVitoN::Core::TCP_Client *sock)
	{
		unsigned char *data = _null_;
		unsigned int data_size;

		while( sock->isActive())
		{
			sock->recv(data, data_size);
            cout << data << endl;
            sock->send(data, data_size);
            // sock->close();
		}

		sock->close();
        GraVitoN::Core::Logger::logItLn("Connection Closed");

		return true;
	}
};

int main()
{
	My_Server server;

	cout << "Main" << endl;
    server.initialize(7357);
	cout << "Main" << endl;
	server.run();
	cout << "End of Main" << endl;

	return 0;
}
