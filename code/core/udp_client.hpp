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
 * @brief GraVitoN::UDP_Client
 *
*/

#ifndef _GVN_UDP_SOCKET_HEAD_
#define _GVN_UDP_SOCKET_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <core/socket.hpp>
#include <external/ting/net/UDPSocket.hpp>
#include <external/ting/mt/Thread.hpp>
#include <utils/netkit.hpp>

namespace GraVitoN
{

namespace Core
{

// typedef ting::net::UDPSocket UDP_Socket_Base;

/// @todo UDP_Socket
class UDP_Socket: public Core::Socket
{
protected:
	/// Socket
    ting::net::UDPSocket sock;

    /// recv packet
    virtual bool doRecv(unsigned char *&data, size_t &data_size, ting::net::IPAddress &sender_addr);

public:
    /// Empty constructor
    UDP_Socket() {}

	/**
     * @brief Create an object of UDP_Socket with a open socket.
	 *
	 * By calling this constructor, there is no need to call initialize
	 */
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
    // UDP_Socket::UDP_Socket(UDP_Socket_Base _sock)
    // {
    //    sock = _sock;
    // }

    virtual ~UDP_Socket();

    /// local_port == 0 means use a random port
    virtual bool open(const unsigned int &local_port = 0);

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

    /// Like recv but also stored ip address and port number of sender
    virtual bool recv(unsigned char *&data, size_t &data_size, string &_sender_ip, unsigned int &_sender_port);

    /// Send data to client
    virtual bool send(const unsigned char *data, const size_t &data_size, const string &ip, const unsigned int &port);

	virtual bool isActive();

    // virtual UDP_Socket_Base getUDPSocketBase() const
    // {
    // 	return sock;
    // }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
UDP_Socket::~UDP_Socket()
{
    if( sock.IsValid() )
        UDP_Socket::close();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Open a port for incomming/outgoing UDP connections
bool UDP_Socket::open(const unsigned int &_local_port)
{
	try
	{
        sock.Open( _local_port );

		ting::WaitSet waitSet(1);
        waitSet.Add(&sock, ting::Waitable::WRITE);
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
bool UDP_Socket::close()
{
	try
	{
        sock.Close();
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
bool UDP_Socket::run()
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
bool UDP_Socket::send(const unsigned char *data, const size_t &data_size, const string &ip, const unsigned int &port)
{
	try
	{
        ting::WaitSet waitSet(1);
        waitSet.Add(&sock, ting::Waitable::WRITE);
        // Logger::logItLn("[Recv UDP] Waiting...");
        waitSet.Wait();

        ting::Buffer<const unsigned char> data_buf(data, (size_t)data_size);
        ting::net::IPAddress m_addr(ip.c_str(), port);
        sock.Send(data_buf, m_addr);
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		return false;
	}

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Socket::doRecv(unsigned char *&data, size_t &data_size, ting::net::IPAddress &sender_addr)
{
	try
	{
		//if ( data )
			//delete data;

		ting::StaticBuffer<unsigned char, MAX_TCP_PACKET_SIZE> data_buf;
        size_t bytes_recved = 0;

        /*
        while( bytes_recved == 0 )
		{
            bytes_recved = sock.Recv(data_buf, addr);
		}
        */

        ting::WaitSet waitSet(1);
        waitSet.Add(&sock, ting::Waitable::READ);
        // Logger::logItLn("[Recv UDP] Waiting...");
        waitSet.Wait();

        bytes_recved = sock.Recv(data_buf, sender_addr);
        if( bytes_recved == 0 )
            return false;
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
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Socket::recv(unsigned char *&data, size_t &data_size)
{
    ting::net::IPAddress sender_addr;
    return doRecv(data, data_size, sender_addr);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Socket::recv(unsigned char *&data, size_t &data_size, string &_sender_ip, unsigned int &_sender_port)
{
    ting::net::IPAddress sender_addr;
    bool res = doRecv(data, data_size, sender_addr);

    if(res)
    {
        _sender_ip = Utils::Netkit::hexToStrIPv4(sender_addr.host);
        _sender_port = sender_addr.port;

        return true;
    }
    return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool UDP_Socket::isActive()
{
    return sock.IsValid();
}

} // Core
} // end of GraVitoN

#endif // _GVN_UDP_SOCKET_HEAD_
