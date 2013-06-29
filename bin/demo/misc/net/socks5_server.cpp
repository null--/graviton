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
 * @brief socks5 server test
 *
 */

#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <utils/socks5_server.hpp>
#include <iostream>
using namespace std;

int main()
{
    // cout << GraVitoN::Utils::Netkit::dnsLookup4("www.google.com") << endl;
    // cout << GraVitoN::Utils::Netkit::dnsLookup4("mail.google.com") << endl;

    // GraVitoN::Utils::Relay_Server_Tcp relay("127.0.0.1", 7357, "127.0.0.1", 80, false);
    // relay.run();

    GraVitoN::Utils::SOCKS5_Server srv(7357);
    // GraVitoN::Utils::SOCKS5_Server srv(7357, "null", "hell");
    srv.run();

	return 0;
}
