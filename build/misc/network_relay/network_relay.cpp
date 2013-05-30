#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/component_parallel.hpp>
#include <utils/network_relay.hpp>
#include <iostream>
using namespace std;

int main()
{
    GraVitoN::Utils::Component_Parallel copa;

    GraVitoN::Core::TCP_Client cln("127.0.0.1", 7356);

    if(!cln.connect())
    {
        cout << "CANNOT CONNECT TO " << cln.getRemoteIP() << ":" << cln.getRemotePort() << endl;
        return 0;
    }

    /// Server: UDP/127.0.0.1:7357
    /// Client: UDP/127.0.0.1:7356
    /// Remote: UDP/127.0.0.1:7358
    GraVitoN::Utils::Network_Relay_UDP nrudp(7357, "127.0.0.1", 7356, "127.0.0.1", 7358);

    /// Server: TCP/127.0.0.1:rand
    /// Client: TCP/127.0.0.1:7356
    /// Remote: UDP/127.0.0.1:7358
    GraVitoN::Utils::Network_Relay_TCP_To_UDP nrtu(cln, "127.0.0.1", 7358);

    copa.runThis( nrudp  );
    copa.runThis( nrtu );

    while( copa.isActive() )
    {
        GraVitoN::Core::Thread::sleep(50);
    }

    cout << "DONE! (But how?!!)" << endl;

	return 0;
}
