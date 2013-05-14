#define GVN_ACTIVATE_LOGGER

#include "core/thread.hpp"
#include "core/udp_client.hpp"
#include <iostream>

class My_Udp_Client: public GraVitoN::Core::UDP_Client
{
public:
	virtual bool run()
	{
		open();

        cout << "sending" <<endl;

		send((const unsigned char *)"Hello World\n", 13);
		unsigned char *data = _null_;
		unsigned int ds;
		recv(data, ds);
        cout<<"Size: "<<ds<<" - "<<data<<endl;

		close();
		return true;
	}
};

int main()
{
    My_Udp_Client udp;

    udp.initialize("127.0.0.1", 7357);
	udp.run();

	return 0;
}
