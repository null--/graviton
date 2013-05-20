/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
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
 * @brief GraVitoN::Lib_Ting
 *
 * @todo IT'S NOT COMPELTED YET
*/

#ifndef _GVN_UDP_SOCKET_HEAD_
#define _GVN_UDP_SOCKET_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <core/socket.hpp>
#include <external/ting/net/UDPSocket.hpp>
#include <external/ting/mt/Thread.hpp>

namespace GraVitoN
{

namespace Core
{

typedef ting::net::UDPSocket UDP_Socket;

/// @todo UDP_Socket
class UDP_Client: public Socket
{
protected:
	/// Create IP address for connecting
	ting::net::IPAddress addr;
	unsigned int local_port;

	/// Socket
	UDP_Socket *sock;

    /**
     * @brief Initialize an UDP client
     *
     * @options
     * IP='Remote IP'
     * PORT='Remote Port'
     * LPORT='Local Port'
     *
     * @note
     * By setting your LPORT to your local port, and IP to '0.0.0.0' you can use UDP_Client as an UDP Listenner,
     * see gravdev/misc/udp_client for more info.
     *
     */
    virtual bool initialize(const string &ip, const unsigned int port, const unsigned int lport = 0);

public:
    UDP_Client(const string &ip, const unsigned int port, const unsigned int lport = 0);
	/**
	 * @brief Create an object of UDP_Client with a open socket.
	 *
	 * By calling this constructor, there is no need to call initialize
	 */
	UDP_Client(UDP_Socket *_sock);

	virtual ~UDP_Client();

	virtual bool open();

	virtual bool close();

	/**
	 * @brief Recieve data
	 *
	 * Do not forget to call open method, before call this function.
	 *
	 * @param [out] data
	 * Recieved data
	 *
	 * @param [out] data_size
	 *
	 * @return true if, something recieved.
	 */
    virtual bool recv(unsigned char *&data, size_t &data_size);

	/**
	 * @brief Recieve data
	 *
	 * Do not forget to call open method, before call this function.
	 *
	 * @param [in] data
	 * Send data
	 *
	 * @param [in] data_size
	 * Size of data
	 *
	 * @return true if data was sended.
	 */
    virtual bool send(const unsigned char *data, const size_t &data_size);

	virtual bool isActive();

	virtual UDP_Socket *getTCPSocket() const
	{
		return sock;
	}
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
UDP_Client::UDP_Client(const string &ip, const unsigned int port, const unsigned int lport)
{
	sock = new UDP_Socket();
    initialize(ip, port, lport);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
UDP_Client::~UDP_Client()
{
	if( sock->IsValid() )
		UDP_Client::close();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Client::initialize(const string &ip, const unsigned int port, const unsigned int lport)
{
    local_port = lport;

	addr = ting::net::IPAddress(ip.c_str(), port);
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Client::open()
{
	try
	{
		/// connect to remote listening socket.
		/// It is an asynchronous operation, so we will use WaitSet
		/// to wait for its completion.
		sock->Open( local_port );

		ting::WaitSet waitSet(1);
		waitSet.Add(sock, ting::Waitable::WRITE);
		waitSet.Wait(); //< Wait for connection to complete.
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		return false;
	}
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Client::close()
{
	try
	{
		sock->Close();
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		return false;
	}
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/*
bool UDP_Client::run()
{
	/// Open socket
	open();

	send((const unsigned char*)"Hello Server", 13);

	unsigned int size;
	unsigned char *data;
	recv(data, size);

	/// Close Socket
	close();

    return true;
}
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Client::send(const unsigned char *data, const size_t &data_size)
{
	try
	{
		ting::Buffer<const unsigned char> data_buf(data, (size_t)data_size);
		sock->Send(data_buf, addr);
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		return false;
	}

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Client::recv(unsigned char *&data, size_t &data_size)
{
	try
	{
		//if ( data )
			//delete data;

		ting::StaticBuffer<unsigned char, MAX_TCP_PACKET_SIZE> data_buf;
        size_t bytes_recved = 0;

		while( bytes_recved == 0 )
		{
			bytes_recved = sock->Recv(data_buf, addr);
		}
		//Logger::logVariable("Bytes Recved: ", bytes_recved);
		data_size = bytes_recved;
		data = new unsigned char[data_size];

        for(size_t i=0; i<bytes_recved; ++i)
			data[i] = data_buf[i];
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		return false;
	}

	return true;
}

bool UDP_Client::isActive()
{
	return sock->IsValid();
}

} // Core
} // end of GraVitoN

#endif // _GVN_UDP_SOCKET_HEAD_
