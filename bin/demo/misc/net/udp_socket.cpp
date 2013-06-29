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
 * @brief test udp socket
 *
 */

#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/udp_socket.hpp>
#include <iostream>
using namespace std;


int main()
{
    GraVitoN::Core::UDP_Socket client;

    // client.open();
    // client.sendString( GraVitoN::Core::UDP_Packet_String("Hello World\n", "127.0.0.1", 7357) );
    // cout << client.recvString().data << endl;
    // client.close();

    client.open();
    client.bind("127.0.0.1", 7358);
    cout << client.recvString().data << endl;
    client.close();
    
    return 0;
}
