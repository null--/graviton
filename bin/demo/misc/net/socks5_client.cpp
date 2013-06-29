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
 * @brief socks5 client test
 *
 */

#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/socks5_client.hpp>
#include <utils/socks5_server.hpp>
#include <iostream>
using namespace std;

int main()
{
    GraVitoN::Utils::SOCKS5_Client soc("127.0.0.1", 7357, "www.google.com", 80, "null", "hell");
    // GraVitoN::Utils::SOCKS5_Client soc("127.0.0.1", 8181, "www.google.com", 80);
    if( soc.connect() )
    {
        string data = "ss\n\n";
        cout<< "SENDING..." << endl;
        if( soc.sendString(data) )
        {
            cout << "RECVING..." << endl;

            // GraVitoN::Core::Thread::sleep(100);

            soc.recvString(data);
            cout << data << endl;
        }
    }
	return 0;
}
