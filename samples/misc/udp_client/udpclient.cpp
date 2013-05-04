#define GVN_ACTIVATE_LOGGER

#include "../../../gvn_utils/gvn_udp_client.hpp"
#include <iostream>

class My_Udp_Client: public GraVitoN::UDP_Client
{
public:
	virtual bool run()
	{
		open();

		send((const unsigned char *)"Hello World\n", 13);
		unsigned char *data = _null_;
		unsigned int ds;
		recv(data, ds);
		cout<<"Sise: "<<ds<<" - "<<data<<endl;

		close();
		return true;
	}
};

class My_Udp_Listener: public GraVitoN::UDP_Client
{
public:
	virtual bool run()
	{
		open();

		//send((const unsigned char *)"Hello World\n", 13);
		unsigned char *data = _null_;
		unsigned int ds;
		recv(data, ds);
		cout<<"Sise: "<<ds<<" - "<<data<<endl;

		close();
		return true;
	}
};

int main()
{
	My_Udp_Client udp;

	udp.initialize("IP=127.0.0.1 PORT=7357");
	udp.run();

	My_Udp_Listener ludp;
	ludp.initialize("IP=0.0.0.0 PORT=7358 LPORT=7357");
	ludp.run();

	return 0;
}
