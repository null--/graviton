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
 * @brief GraVitoN::TCP_Client
 *
 * @todo IT'S NOT COMPELTED YET
*/

#ifndef _GVN_TCP_CLIENT_HEAD_
#define _GVN_TCP_CLIENT_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <core/socket.hpp>
#include <external/ting/net/TCPSocket.hpp>
#include <external/ting/net/TCPServerSocket.hpp>
#include <external/ting/mt/Thread.hpp>

namespace GraVitoN
{

namespace Core
{

typedef ting::net::TCPSocket TCP_Socket;

/// @brief TCP Client Component
class TCP_Client : public Socket, public GraVitoN::Core::Component
{
private:
	/// Do not copy
	//TCP_Client & operator = (const TCP_Client &_copy);

protected:
	//ting::net::Lib *socket_lib;

	/// Create IP address for connecting
	ting::net::IPAddress addr;

	/// Socket
	TCP_Socket *sock;

public:
	TCP_Client();
	/**
	 * @brief Create an object of TCP_Client with a open socket.
	 *
	 * By calling this constructor, there is no need to call initialize
	 */
	TCP_Client(TCP_Socket *_sock);

	virtual ~TCP_Client();

	/**
	 * @brief Initialize an TCP Client
	 *
	 * @options
	 * IP='Remote IP'
	 * PORT='Remote Port'
	 *
	 */
    virtual bool initialize(const string ip, unsigned int port);

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
	virtual bool recv(unsigned char *&data, unsigned int &data_size);

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
	virtual bool send(const unsigned char *data, const unsigned int &data_size);

	/**
	 * @brief main loop
	 *
	 * You can override this method, and use them after initilialize you inherited
	 * object. DO NOT forget to call open/close methods.
	 */
	virtual bool run();

	virtual bool isActive();

	virtual TCP_Socket *getTCPSocket() const
	{
		return sock;
	}
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Client::TCP_Client()
{
	sock = new TCP_Socket();
}

TCP_Client::TCP_Client(TCP_Socket *_sock)
{
	TCP_Client();
	sock = _sock;
	addr = sock->GetRemoteAddress();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Client::~TCP_Client()
{
	if( sock->IsValid() )
		TCP_Client::close();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::initialize(const string ip, unsigned int port)
{
	addr = ting::net::IPAddress(ip.c_str(), port);
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::open()
{
	try
	{
		/// connect to remote listening socket.
		/// It is an asynchronous operation, so we will use WaitSet
		/// to wait for its completion.
		sock->Open(addr);

		ting::WaitSet waitSet(1);
		waitSet.Add(sock, ting::Waitable::WRITE);
		waitSet.Wait(); //< Wait for connection to complete.
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		sock->Close();
		return false;
	}
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::close()
{
	try
	{
		sock->Close();
		//delete sock;
		//sock = _null_;
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		return false;
	}
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::run()
{
	/// Open socket
	open();

	send((const unsigned char*)"Hello Server", 13);

	unsigned int size;
	unsigned char *data;
	recv(data, size);

	/// Close Socket
	close();

    return size != 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::send(const unsigned char *data, const unsigned int &data_size)
{
	try
	{
		ting::Buffer<const unsigned char> data_buf(data, (size_t)data_size);
		sock->Send(data_buf);
		//ting::mt::Thread::Sleep(1);
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		sock->Close();
		return false;
	}
	/*
	catch(char * exp)
	{
		Logger::logItLn(exp);
		return false;
	}
	*/
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::recv(unsigned char *&data, unsigned int &data_size)
{
	try
	{
		if ( data )
		{
			delete data;
		}

		ting::StaticBuffer<unsigned char, MAX_TCP_PACKET_SIZE> data_buf;
		unsigned int bytes_recved = 0;

		while( bytes_recved == 0 )
		{
			bytes_recved += sock->Recv(data_buf, bytes_recved);
			ting::mt::Thread::Sleep(1);
		}
		//Logger::logVariable("Bytes Recved: ", bytes_recved);
		data_size = bytes_recved;
		data = new unsigned char[data_size];

        for(size_t i=0; i<bytes_recved; ++i)
			data[i] = data_buf[i];

		//ting::mt::Thread::Sleep(1);
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
		sock->Close();
		return false;
	}

	return true;
}

bool TCP_Client::isActive()
{
	try
	{
		if(!sock) return false;
		return sock->IsValid();
	}
	catch(...)
	{
		return false;
	}
	return false;
}

//TCP_Client & TCP_Client::operator =(const TCP_Client &_copy)
//{
//	if( this == &_copy ) return *this;

//	info = _copy.getInfo();
//	options = _copy.getOptions();
//	sock = _copy.getTCPSocket();
//	//sockets_lib = _copy.sockets_lib;
//	addr = this->sock->GetRemoteAddress();
//	return *this;
//}

} // Core
} // end of GraVitoN

#endif // _GVN_TCP_CLIENT_HEAD_
