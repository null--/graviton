/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief test Network Relay
 *
 */

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
    }

    /// relay : UDP/127.0.0.1:7357
    /// Client: UDP/127.0.0.1:7356
    /// Remote: UDP/127.0.0.1:7358
    GraVitoN::Utils::Network_Relay_UDP nrudp("0.0.0.0", 7357, "127.0.0.1", 7356, "127.0.0.1", 7358);

    /// relay : TCP/127.0.0.1:rand
    /// Client: TCP/127.0.0.1:7356
    /// Remote: UDP/127.0.0.1:7358
    GraVitoN::Utils::Network_Relay_TCP_To_UDP nrtu(cln, "0.0.0.0", "127.0.0.1", 7358);

    /// ms08_67
    GraVitoN::Utils::Network_Relay_TCP_Server ms08_67("0.0.0.0", 31337, "192.168.56.102", 445);

    /// Meterpreter Bind
    GraVitoN::Utils::Network_Relay_TCP_Server meter_bnd("0.0.0.0", 4444, "192.168.56.102", 4444);

    copa.runThis( nrudp  );
    copa.runThis( nrtu );
    copa.runThis( ms08_67 );
    copa.runThis( meter_bnd );

    while( copa.isActive() )
    {
        GraVitoN::Core::sleep(50);
    }

    cout << "DONE! (But how?!!)" << endl;

	return 0;
}
