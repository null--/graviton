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
/// Base of TCP_Server
/// It's NOT a GraVitoN::Component
class TCP_Server_Base: public GraVitoN::Core::Socket
{
protected:
    unsigned int port;

    /// Socket
    ting::net::TCPServerSocket listenSock;

    //ting::net::Lib *socket_lib;

public:
    TCP_Server_Base(unsigned int _port)
    {
        port = _port;
    }

    ~TCP_Server_Base() throw()
    {
        close();
    }

    /// Listen socket
    TCP_Client accept();

    /// Open socket
    virtual bool open();

    /// Open socket
    virtual bool close();

    virtual bool isActive();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief TCP Server Component
 */
class TCP_Server : public TCP_Server_Base ,public GraVitoN::Core::Component // Component_With_Init
{
private:
    bool multi_thread;

protected:

    /// Response function
    virtual bool response(GraVitoN::Core::TCP_Client &client_sock) = 0;

    /// Server socket thread
    class SERVER_INTERNAL_THREAD : public GraVitoN::Core::Thread
    {
    private:
        TCP_Server &server_handle;
        TCP_Client &sock;

    protected:
        bool myMainLoop()
        {
            server_handle.response(sock);
            return true;
        }

    public:
        SERVER_INTERNAL_THREAD(TCP_Server &_this_server, TCP_Client &client_sock)
            :server_handle(_this_server),
            sock(client_sock)
        {
            // server_handle = _this_server;
            // sock = client_sock;
        }

        virtual ~SERVER_INTERNAL_THREAD()throw()
        {
            sock.close();
        }
    };
    //std::vector<SERVER_INTERNAL_THREAD> internal_threads;
    list<SERVER_INTERNAL_THREAD*> internal_threads;

public:
    TCP_Server(const unsigned int local_port, const bool enable_multi_thread = true);

    virtual ~TCP_Server() throw();

    /// Close socket
    virtual bool close();

    /// Listen socket
    virtual bool listen();

    /**
     * @brief main loop
     *
     * You can redifine this method, or send/recv methods; and use them after initilialize you inherited
     * object.
     */
    virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Server::TCP_Server(unsigned int local_port, const bool enable_multi_thread) : TCP_Server_Base(local_port)
{
    multi_thread = enable_multi_thread;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Server::~TCP_Server() throw()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server_Base::open()
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
TCP_Client TCP_Server_Base::accept()
{
    TCP_Socket sock;
    while( (sock = listenSock.Accept() ).IsNotValid() )
    {
        ting::mt::Thread::Sleep(1);
    }

    return TCP_Client(sock);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::listen()
{
    try
    {
        if( multi_thread )
        {
            Core::Logger::logItLn("Listening (MultiThread)...");

            // list<TCP_Socket> lsock;
            list<TCP_Client> client_sock;

            while(listenSock.IsValid())
            //if(listenSock.IsValid())
            {
                /// Check for waiting connection

                // lsock.push_back ( accept() );
                client_sock.push_back( accept() );

                /// Initialize and run response thread.
                internal_threads.push_back( new SERVER_INTERNAL_THREAD((TCP_Server&)*this, client_sock.back() ));
                internal_threads.back()->run();
            }
        }
        else
        {
            Core::Logger::logItLn("Listening (Single Thread)...");
            while( listenSock.IsValid() )
            {
                ting::mt::Thread::Sleep(1);

                TCP_Client cln = accept();
                this->response( cln );
                break;
            }
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
bool TCP_Server_Base::close()
{
    listenSock.Close();
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server::close()
{
    try
    {
        TCP_Server_Base::close();

        if( !internal_threads.empty() )
        {
            for(list<SERVER_INTERNAL_THREAD*>::iterator p_trd = internal_threads.begin(); p_trd != internal_threads.end(); ++p_trd)
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

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server_Base::isActive()
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

} // end of Core
} // end of GraVitoN

#endif // _GVN_TCP_SERVER_HEAD_
