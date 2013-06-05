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
    ting::net::TCPServerSocket listen_sock;

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

    /// Things that should be done before calling response (e.g. SSL Handshake)
    virtual bool initializeConnection(GraVitoN::Core::TCP_Client &client_sock)
    {
        /// Sample Code
        if( !client_sock.isActive() )
            return false;
        return true;
    }

    /// Things that should be done before calling response (e.g.
    virtual bool finalizeConnection(GraVitoN::Core::TCP_Client &client_sock)
    {
        /// Sample Code
        if( client_sock.isActive() )
            client_sock.close();
        return true;
    }

    /// Server socket thread
    class SERVER_INTERNAL_THREAD : public GraVitoN::Core::Thread
    {
    private:
        TCP_Server &server_handle;
        TCP_Client &sock;

    protected:
        bool myMainLoop()
        {
            try
            {
                if( !server_handle.initializeConnection(sock) )
                {
                    sock.close();
                    return false;
                }

                bool res = server_handle.response(sock);

                if( !server_handle.finalizeConnection(sock) )
                {
                    sock.close();
                    return false;
                }

                return res;
            }
            catch(ting::net::Exc &e)
            {
                Logger::logVariable("Network error", e.What());
                sock.close();
            }

            return false;
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
        listen_sock.Open(port);

        //ting::WaitSet waitSet(1);
        //waitSet.Add(&listen_sock, ting::Waitable::WRITE);
        //waitSet.Wait(); //< Wait for connection to complete.

        Logger::logItLn("done");
    }
    catch(ting::net::Exc &e)
    {
        Logger::logVariable("Network error", e.What());
        listen_sock.Close();
        return false;
    }
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
TCP_Client TCP_Server_Base::accept()
{
    TCP_Socket sock;
    // while( (sock =listen_sock.Accept()).IsNotValid() )
    {
        ting::WaitSet waitSet(1);
        waitSet.Add(&listen_sock, ting::Waitable::READ);
        waitSet.Wait(); //< Wait for connection to complete.

        sock = listen_sock.Accept();

        // ting::mt::Thread::Sleep(1);
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

            while(listen_sock.IsValid())
            //if(listen_sock.IsValid())
            {
                /// @todo Bugfix: nmap -sV causes crash! (this try/catch solution is not geek!)
                try
                {
                    /// Check for waiting connection

                    // lsock.push_back ( accept() );
                    client_sock.push_back( accept() );

                    /// Initialize and run response thread.
                    internal_threads.push_back( new SERVER_INTERNAL_THREAD((TCP_Server&)*this, client_sock.back() ));
                    // cout << "Running..." << endl;
                    internal_threads.back()->run();
                }
                catch(ting::net::Exc &e)
                {
                    Logger::logVariable("[Listen Loop] Network error", e.What());
                    close();
                    open();
                }
            }
        }
        else
        {
            Core::Logger::logItLn("Listening (Single Thread)...");
            while( listen_sock.IsValid() )
            {
                ting::mt::Thread::Sleep(1);

                try
                {
                    TCP_Client cln = accept();
                    if( this->initializeConnection( cln ) )
                        if( this->response( cln ) )
                            this->finalizeConnection( cln );
                }
                catch(ting::net::Exc &e)
                {
                    Logger::logVariable("[Listen Loop] Network error", e.What());
                }
            }
        }
    }
    catch(ting::net::Exc &e)
    {
        Logger::logVariable("[Listen] Network error", e.What());
        listen_sock.Close();
        return false;
    }

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool TCP_Server_Base::close()
{
    listen_sock.Close();
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
        return listen_sock.IsValid();
    }
    catch(...)
    {
        return false;
    }
    return false;
}

} // end of Core

//=============================================================================//
#ifdef GVN_ACTIVATE_LUABRIDGE

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
namespace LUABridge
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class LUATCPServer : public Core::TCP_Server
{
private:
    luabridge::LuaRef lua_response;

protected:
    virtual bool response(Core::TCP_Client &client_sock)
    {
        return lua_response(client_sock);
    }

public:
    LUATCPServer(const unsigned int local_port, luabridge::LuaRef callback, const bool enable_multi_thread = true):
        TCP_Server(local_port, enable_multi_thread),
        lua_response(callback)
    {

    }

    ~LUATCPServer() throw()
    {

    }

    virtual bool run()
    {
        return Core::TCP_Server::run();
    }

    virtual bool isActive()
    {
        return Core::TCP_Server::isActive();
    }

    virtual bool close()
    {
        return Core::TCP_Server::close();
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
void addClass_TCP_Server()
{
    Core::Logger::logItLn("Loaing GraVitoN.Core.TCP_Server");

    luabridge::getGlobalNamespace ( Core::Luaviton::getInstance().getState() )
            .beginNamespace("GraVitoN")
            .beginNamespace("Core")
            .beginClass <LUATCPServer> ("TCP_Server")
            .addConstructor < void(*) (const unsigned int, luabridge::LuaRef, const bool), RefCountedPtr<LUATCPServer> > ()
            .addFunction("run", &LUATCPServer::run)
            .addFunction("isActive", &LUATCPServer::isActive)
            .addFunction("close", &LUATCPServer::close)
            .endClass()
            .endNamespace()
            .endNamespace()
            ;
}

} // LUABridge
#endif
} // end of GraVitoN

#endif // _GVN_TCP_SERVER_HEAD_
