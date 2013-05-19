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
 * @brief GraVitoN::TCP_Server
 *
 * @todo IT'S NOT COMPELTED YET
*/

#ifndef _GVN_TCP_SERVER_HEAD_
#define _GVN_TCP_SERVER_HEAD_

#include <list>

#include <graviton.hpp>
#include <core/tcp_client.hpp>
#include <core/logger.hpp>
#include <core/socket.hpp>
#include <external/ting/net/TCPSocket.hpp>
#include <external/ting/net/TCPServerSocket.hpp>
#include <core/thread.hpp>

namespace GraVitoN
{

namespace Core
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief TCP Server Component
 */
class TCP_Server : public GraVitoN::Core::Socket, public GraVitoN::Core::Component // Component_With_Init
{
protected:
    /// Response function
    virtual bool response(GraVitoN::Core::TCP_Client *client_sock);

    /// Server socket thread
    class Server_Thread : public GraVitoN::Core::Thread
    {
    private:
        TCP_Server *server_handle;
        TCP_Client *sock;

    protected:
        bool myMainLoop()
        {
            server_handle->response(sock);
            return true;
        }

    public:
        Server_Thread(TCP_Server *_this_server, TCP_Client *client_sock)
        {
            server_handle = _this_server;
            sock = client_sock;
        }

        virtual ~Server_Thread()throw()
        {
            sock->close();
        }
    };
    //std::vector<Server_Thread> internal_threads;
    list<Server_Thread*> internal_threads;

protected:
    /// Create IP address for connecting
    unsigned int port;

    /// Socket
    ting::net::TCPServerSocket listenSock;

    //ting::net::Lib *socket_lib;

public:
    TCP_Server();

    virtual ~TCP_Server() throw();

    /**
     * @brief Initialize an TCP Server
     *
     * @options
     * unsigned int: port
     *
     */
    virtual bool initialize(unsigned int local_port); // initialize(...);

    /// Open socket
    virtual bool open();

    /// Close socket
    virtual bool close();

    /// Listen socket
    virtual bool listen();

/// Send and Recieve a packet should be handled by a TCP_Client object
//	/**
//	 * @brief Recieve data
//	 *
//	 * Do not forget to call open method, before call this function.
//	 *
//	 * @param [out] data
//	 * Recieved data
//	 *
//	 * @param [out] data_size
//	 *
//	 * @return true if, something recieved.
//	 */
//	static bool recv(TCP_Socket &sock, unsigned char *&data, unsigned int &data_size);

//	/**
//	 * @brief Recieve data
//	 *
//	 * Do not forget to call open method, before call this function.
//	 *
//	 * @param [in] data
//	 * Send data
//	 *
//	 * @param [in] data_size
//	 * Size of data
//	 *
//	 * @return true if data was sended.
//	 */
//	static bool send(TCP_Socket &sock, const unsigned char *data, const unsigned int &data_size);

    /**
     * @brief main loop
     *
     * You can redifine this method, or send/recv methods; and use them after initilialize you inherited
     * object.
     */
    virtual bool run();

    virtual bool isActive();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Server::TCP_Server()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Server::~TCP_Server() throw()
{
    TCP_Server::close();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::initialize(unsigned int local_port) // initialize(...)
{
    if( listenSock.IsValid() )
        this->close();

    // va_list vl;
    // va_start(vl, 1); /// Only one argument
    // port = va_arg(vl, unsigned int);
    // va_end(vl);

    port = local_port;

    //if( !OptParser::getValueAsUInt(options, "THREADS", max_threads) )
    //return false;

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::open()
{
    try
    {
        Logger::logItLn("Openning...");
        Logger::logVariable("Port", port);

        /// connect to remote listening socket.
        /// It is an asynchronous operation, so we will use WaitSet
        /// to wait for its completion.
        listenSock.Open(port);

        //ting::WaitSet waitSet(1);
        //waitSet.Add(&listenSock, ting::Waitable::WRITE);
        //waitSet.Wait(); //< Wait for connection to complete.

        Logger::logItLn("done");
    }
    catch(ting::net::Exc &e)
    {
        Logger::logVariable("Network error", e.What());
        listenSock.Close();
        return false;
    }
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::listen()
{
    try
    {
        Core::Logger::logItLn("Listening...");

        list<TCP_Client*> client_sock;
        list<TCP_Socket> lsock;
        while(listenSock.IsValid())
        //if(listenSock.IsValid())
        {
            ting::mt::Thread::Sleep(1);

            /// Check for waiting connection
            TCP_Socket sck = listenSock.Accept();

            /// Validate socket
            if( !sck.IsValid() )
                continue;
            lsock.push_back(sck);

            client_sock.push_back(new TCP_Client( &(lsock.back()) ));

            /// Initialize and run response thread.
            internal_threads.push_back( new Server_Thread(this, client_sock.back()) );
            internal_threads.back()->run();
        }
    }
    catch(ting::net::Exc &e)
    {
        Logger::logVariable("Network error", e.What());
        listenSock.Close();
        return false;
    }

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::close()
{
    try
    {
        listenSock.Close();

        if( !internal_threads.empty() )
        {
            for(list<Server_Thread*>::iterator p_trd = internal_threads.begin(); p_trd != internal_threads.end(); ++p_trd)
                (*p_trd)->stop();
            internal_threads.clear();
        }
    }
    catch(ting::net::Exc &e)
    {
        Logger::logVariable("Network error", e.What());
        return false;
    }
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::run()
{
    /// Open socket
    open();

    /// Listen
    listen();

    /// Close Socket
    close();

    return true;
}

////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
//bool TCP_Server::send(TCP_Socket &sock, const unsigned char *data, const unsigned int &data_size)
//{
//	try
//	{
//		ting::Buffer<const unsigned char> data_buf(data, (size_t)data_size);
//		sock.Send(data_buf);
//	}
//	catch(ting::net::Exc &e)
//	{
//		Logger::logVariable("Network error", e.What());
//		return false;
//	}

//	return true;
//}

////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
//bool TCP_Server::recv(TCP_Socket &sock, unsigned char *&data, unsigned int &data_size)
//{
//	try
//	{
//		if ( data )
//			delete data;

//		ting::StaticBuffer<unsigned char, MAX_TCP_PACKET_SIZE> data_buf;
//		unsigned int bytes_recved = 0;

//		while( bytes_recved == 0 )
//		{
//			bytes_recved += sock.Recv(data_buf, bytes_recved);
//		}
//		//Logger::logVariable("Bytes Recved: ", bytes_recved);
//		data_size = bytes_recved;
//		data = new unsigned char[data_size];

//		for(int i=0; i<bytes_recved; ++i)
//			data[i] = data_buf[i];
//	}
//	catch(ting::net::Exc &e)
//	{
//		Logger::logVariable("Network error", e.What());
//		return false;
//	}

//	return true;
//}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::isActive()
{
    try
    {
        return listenSock.IsValid();
    }
    catch(...)
    {
        return false;
    }
    return false;
}

bool TCP_Server::response(GraVitoN::Core::TCP_Client *client_sock)
{
    client_sock->send((const unsigned char*)"Hello Client", 13);
    return true;
}

} // end of Core
} // end of GraVitoN

#endif // _GVN_TCP_SERVER_HEAD_
