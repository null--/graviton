#define GVN_ACTIVATE_LOGGER

#include "graviton.hpp"
#include "core/tcp_server.hpp"
#include <iostream>
using namespace std;

int max_try = 3;

class My_Server : public GraVitoN::Core::TCP_Server
{
public:
    virtual bool response(GraVitoN::Core::TCP_Client &sock)
	{
        /// @warning race condition
        --max_try;

		unsigned char *data = _null_;
        size_t data_size;

        while( sock.isActive())
		{
            sock.recv(data, data_size);
            cout << data << endl;
            sock.send(data, data_size);

            if( strcmp((char *)data, "\n") == 0 )
            {
                sock.close();
                break;
            }
		}

        GraVitoN::Core::Logger::logItLn("Connection Closed");

        if( max_try <= 0 )
            My_Server::close();

		return true;
	}

    My_Server(unsigned int port) : TCP_Server(port)
    {
    }
};

int main()
{
    My_Server server(7357);

	cout << "Main" << endl;
	server.run();
	cout << "End of Main" << endl;

	return 0;
}
