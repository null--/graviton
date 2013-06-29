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
 * @brief test tcp server
 *
 */

#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/tcp_server.hpp>
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

        cout << "Connection Established!" << endl;
        while( sock.isActive() )
		{
            std::string data;
            data = sock.recvString();

            cout << data << endl;
            sock.sendString(data);

            if( data == "\n" )
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

    My_Server(const string &ip, unsigned int port) : TCP_Server(ip, port)
    {
    }
};

int main()
{
    My_Server server("0.0.0.0", 7357);

	cout << "Main" << endl;
	server.run();
	cout << "End of Main" << endl;

	return 0;
}
