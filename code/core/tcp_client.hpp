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
*/

#ifndef _GVN_TCP_CLIENT_HEAD_
#define _GVN_TCP_CLIENT_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <core/socket.hpp>
#include <utils/netkit.hpp>
#include <external/ting/net/TCPSocket.hpp>
#include <external/ting/net/TCPServerSocket.hpp>
#include <external/ting/mt/Thread.hpp>

namespace GraVitoN
{

namespace Core
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class TCP_Socket : public ting::net::TCPSocket
{
public:
    typedef ting::net::Exc Exception;

public:
    TCP_Socket() : ting::net::TCPSocket()
    {

    }

    TCP_Socket(const ting::net::TCPSocket &_tsock) : ting::net::TCPSocket(_tsock)
    {

    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @brief TCP Client Component
class TCP_Client : public Core::Socket//, public GraVitoN::Core::Component
{
//private:
	/// Do not copy
	//TCP_Client & operator = (const TCP_Client &_copy);

protected:
    bool is_dead;
	//ting::net::Lib *socket_lib;

	/// Create IP address for connecting
	ting::net::IPAddress addr;

	/// Socket
    TCP_Socket sock;

    /**
     * @brief Initialize an TCP Client
     *
     * @options
     * IP='Remote IP'
     * PORT='Remote Port'
     *
     */
    virtual bool initialize(const string ip, unsigned int port);

public:
    TCP_Client(const string &ip, const unsigned int port);
	/**
	 * @brief Create an object of TCP_Client with a open socket.
	 *
	 * By calling this constructor, there is no need to call initialize
	 */
    TCP_Client(TCP_Socket _sock);

	virtual ~TCP_Client();

    virtual bool connect();

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

    virtual string recvString();

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

    virtual bool sendString(const string &data);

	virtual bool isActive();

    /// HIGHLY DANGEROUS
    TCP_Socket *getTCPSocket()
	{
        return &sock;
	}

    string getRemoteIP()
    {
        return Utils::Netkit::hexToStrIPv4( sock.GetRemoteAddress().host );
    }

    unsigned int getRemoteIPHex()
    {
        return sock.GetRemoteAddress().host;
    }

    unsigned int getRemotePort()
    {
        return sock.GetRemoteAddress().port;
    }

    string getLocalIP()
    {
        return Utils::Netkit::hexToStrIPv4( sock.GetLocalAddress().host );
    }

    unsigned int getLocalIPHex()
    {
        return sock.GetLocalAddress().host;
    }

    unsigned int getLocalPort()
    {
        return sock.GetLocalAddress().port;
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Client::TCP_Client(const string &ip, unsigned int port)
{
    // sock = new TCP_Socket();
    initialize(ip, port);
}

TCP_Client::TCP_Client(TCP_Socket _sock)
{
    sock = _sock;
    addr = sock.GetRemoteAddress();
    is_dead = sock.IsNotValid();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Client::~TCP_Client()
{
    if( sock.IsValid() )
		TCP_Client::close();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::initialize(const string ip, unsigned int port)
{
    is_dead = true;
	addr = ting::net::IPAddress(ip.c_str(), port);
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::connect()
{
	try
	{
		/// connect to remote listening socket.
		/// It is an asynchronous operation, so we will use WaitSet
		/// to wait for its completion.
        sock.Open(addr);

		ting::WaitSet waitSet(1);
        waitSet.Add(&sock, ting::Waitable::WRITE);
		waitSet.Wait(); //< Wait for connection to complete.
	}
	catch(ting::net::Exc &e)
	{
		Logger::logVariable("Network error", e.What());
        sock.Close();

        is_dead = false;
		return false;
	}

    is_dead = sock.IsNotValid();
    return sock.IsValid();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::close()
{
    is_dead = true;

	try
	{
        sock.Close();
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
//bool TCP_Client::run()
//{
//	/// Open socket
//	connect();

//	send((const unsigned char*)"Hello Server", 13);

//    size_t size;
//	unsigned char *data;
//	recv(data, size);

//	/// Close Socket
//	close();

//    return size != 0;
//}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::send(const unsigned char *data, const size_t &data_size)
{
    if(!isActive()) return false;

	try
	{
		ting::Buffer<const unsigned char> data_buf(data, (size_t)data_size);
        ting::WaitSet waitSet(1);
        waitSet.Add(&sock, ting::Waitable::WRITE);
        // Logger::logItLn("[Send] Waiting...");
        waitSet.Wait();

        sock.Send(data_buf);
		//ting::mt::Thread::Sleep(1);
	}
	catch(ting::net::Exc &e)
	{
        Logger::logVariable("[TCP_Client] Network error", e.What());
        // sock->Close();
        close();
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
bool TCP_Client::sendString(const string &data)
{
    return TCP_Client::send((unsigned char*)data.c_str(), data.size());
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
string TCP_Client::recvString()
{
    // sdata = "";

    unsigned char *data = _null_;
    size_t data_size;
    if( TCP_Client::recv(data, data_size) )
    {
        /*
        for(size_t i = 0; i<data_size; ++i)
        {
            sdata += (char)data[i];
        }
        return true;
        */
        return string((char*)data);
    }

    return "";
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::recv(unsigned char *&data, size_t &data_size)
{
    if(!isActive()) return false;

    size_t bytes_recved = 0;

	try
	{
        /// @todo to free or not to free...
		if ( data )
		{
			delete data;
		}

		ting::StaticBuffer<unsigned char, Config::MAX_TCP_PACKET_SIZE> data_buf;

        /*
        while( bytes_recved == 0 )
        {
            bytes_recved += sock.Recv(data_buf, bytes_recved);
            // ting::mt::Thread::Sleep(1);
		}
        */
        ting::WaitSet waitSet(1);
        waitSet.Add(&sock, ting::Waitable::READ);

        // Logger::logItLn("[Recv] Waiting...");
        waitSet.Wait();
        bytes_recved = sock.Recv(data_buf, bytes_recved);

        // Logger::logVariable("Bytes Recved: ", bytes_recved);
		data_size = bytes_recved;
        data = new unsigned char[data_size + 1];

        for(size_t i=0; i<bytes_recved; ++i)
			data[i] = data_buf[i];

        data[data_size] = '\0';
		//ting::mt::Thread::Sleep(1);
	}
	catch(ting::net::Exc &e)
	{
        Logger::logVariable("[TCP_Client] Network error", e.What());
        // sock->Close();
        close();
		return false;
	}

    if( bytes_recved == 0 )
        close();
    return bytes_recved != 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Client::isActive()
{
	try
	{
        // if(!sock) return false;
        // std::cout << (!is_dead && sock.IsValid()) << endl;
        return !is_dead && sock.IsValid();
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

//=============================================================================//
#ifdef GVN_ACTIVATE_LUABRIDGE
namespace LUABridge
{
void addClass_TCP_Client()
{
    luabridge::getGlobalNamespace ( Core::Luaviton::getInstance().getState() )
            .beginNamespace("GraVitoN")
            .beginNamespace("Core")
            .beginClass <Core::TCP_Socket> ("TCP_Socket")
            .addConstructor < void(*) (), RefCountedPtr<Core::TCP_Socket> > ()
            .endClass()
            .beginClass <Core::TCP_Client> ("TCP_Client")
            .addConstructor < void(*) (const string&, const unsigned int), RefCountedPtr<Core::TCP_Client> > ()
            // .addConstructor < void(*) (Core::TCP_Socket), RefCountedPtr<Core::TCP_Client> > ()
            .addFunction("connect", &Core::TCP_Client::connect)
            .addFunction("close", &Core::TCP_Client::close)
            .addFunction("sendString", &Core::TCP_Client::sendString)
            .addFunction("recvString", &Core::TCP_Client::recvString)
            .addFunction("isActive", &Core::TCP_Client::isActive)
            .addFunction("getLocalIP", &Core::TCP_Client::getLocalIP)
            .addFunction("getLocalIPHex", &Core::TCP_Client::getLocalIPHex)
            .addFunction("getLocalPort", &Core::TCP_Client::getLocalPort)
            .addFunction("getRemoteIP", &Core::TCP_Client::getRemoteIP)
            .addFunction("getRemoteIPHex", &Core::TCP_Client::getRemoteIPHex)
            .addFunction("getRemotePort", &Core::TCP_Client::getRemotePort)
            .endClass()
            .endNamespace()
            .endNamespace()
            ;
}
}
#endif
} // end of GraVitoN

#endif // _GVN_TCP_CLIENT_HEAD_
