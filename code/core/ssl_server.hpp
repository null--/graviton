#ifndef _GVN_SSL_SERVER_HEAD_
#define _GVN_SSL_SERVER_HEAD_

#include <graviton.hpp>
#include <core/ssl_socket.hpp>
#include <core/ssl_client.hpp>
#include <core/thread.hpp>
#include <list>

using namespace  std;

namespace GraVitoN
{
namespace Core
{

/// @todo SSL Server
class SSL_Server : public Core::Component
{
protected:
    unsigned int port;
    bool multi_thread;

    string cert_file;
    string key_file;
    // SSL_CTX* ctx;

    struct sockaddr_in sa_serv;

    SSL_Socket::Handle listen_sock;

    /// Server socket thread
    class SERVER_INTERNAL_THREAD : public GraVitoN::Core::Thread
    {
    private:
        SSL_Server &server_handle;
        SSL_Client &sock;

    protected:
        bool myMainLoop()
        {
            try
            {
//                if( !server_handle.initializeConnection(sock) )
//                {
//                    sock.close();
//                    return false;
//                }

                bool res = server_handle.response(sock);

                if( !server_handle.finalizeConnection(sock) )
                {
                    sock.close();
                    return false;
                }

                return res;
            }
            catch(...)
            {
                Logger::logItLn("[SSL Server] Client Thread - Exception");
                sock.close();
            }

            return false;
        }

    public:
        SERVER_INTERNAL_THREAD(SSL_Server &_this_server, SSL_Client &client_sock)
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
    list<SERVER_INTERNAL_THREAD*> internal_threads;

public:
    virtual bool initializeConnection(GraVitoN::Core::SSL_Client &client_sock);
    virtual bool response(GraVitoN::Core::SSL_Client &client_sock) = 0;
    virtual bool finalizeConnection(GraVitoN::Core::SSL_Client &client_sock);

public:
    SSL_Server(const unsigned int local_port,
               const string &_cert_file,
               const string &_key_file,
               const bool enable_multi_thread = true)
    {
        SSL_Socket::getInstance();

        cert_file = _cert_file;
        key_file = _key_file;
        multi_thread = enable_multi_thread;
        port = local_port;

        listen_sock = socket (AF_INET, SOCK_STREAM, 0);
    }

    virtual bool run();
    virtual bool open();
    virtual bool listen();
    virtual bool accept(SSL_Socket::Handle &handle, SSL_Socket::Address &addr);
    virtual bool close();
};

bool SSL_Server::run()
{
    if( open() )
    {
        if( listen() )
        {
            close();
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool SSL_Server::open()
{
    try
    {
        memset (&sa_serv, '\0', sizeof(sa_serv));
        sa_serv.sin_family      = AF_INET;
        sa_serv.sin_addr.s_addr = INADDR_ANY;
        sa_serv.sin_port        = htons (port);          /* Server Port number */

        int err = ::bind(listen_sock, (struct sockaddr*) &sa_serv, sizeof (sa_serv));
        if( err == SSL_Socket::socketError() )
        {
            Core::Logger::logItLn("[SSL SERVER] bind failed");
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool SSL_Server::listen()
{
    try
    {
        /* Receive a TCP connection. */
        int err = ::listen (listen_sock, 5);
        if( err == SSL_Socket::socketError() )
        {
            Core::Logger::logItLn("[SSL SERVER] listen failed");
            return false;
        }

        if( multi_thread )
        {
            Core::Logger::logItLn("[SSL Server] Listening (MultiThread)...");

            // list<TCP_Socket> lsock;
            list<SSL_Client*> client_sock;

            while(listen_sock != SSL_Socket::invalidSocket())
            {
                try
                {
                    SSL_Socket::Handle clis;
                    SSL_Socket::Address addr;

                    if( !accept(clis, addr) )
                        continue;

                    // SSL_Client sslcli(clis, addr);

                    client_sock.push_back( new SSL_Client(clis, addr) );
                    initializeConnection(*client_sock.back());
                    /// Initialize and run response thread.
                    internal_threads.push_back( new SERVER_INTERNAL_THREAD((SSL_Server&)*this, *client_sock.back() ));
                    // cout << "Running..." << endl;
                    internal_threads.back()->run();
                }
                catch(...)
                {
                    Logger::logItLn("[SSL Server] Listen Loop - Exception");
                }
            }
        }
        else
        {
            Core::Logger::logItLn("[SSL Server] Listening (Single Thread)...");
            while(listen_sock != SSL_Socket::invalidSocket())
            {
                ting::mt::Thread::Sleep(1);

                SSL_Socket::Handle clis;
                SSL_Socket::Address addr;

                if( !accept(clis, addr) )
                    continue;

                SSL_Client cln(clis, addr);

                try
                {
                    if( this->initializeConnection( cln ) )
                        if( this->response( cln ) )
                            this->finalizeConnection( cln );
                }
                catch(...)
                {
                    Logger::logItLn("[SSL Server] Listen Loop - Exception");
                }
            }
        }
    }
    catch(...)
    {
        Logger::logItLn("[SSL Server] Listen Loop - Exception");
        close();
        return false;
    }

    return true;
}

bool SSL_Server::accept(SSL_Socket::Handle &handle, SSL_Socket::Address &addr)
{
    try
    {
        SSL_Socket::Size client_len = sizeof(addr);
        handle = ::accept (listen_sock, (struct sockaddr*) &addr, &client_len);
        if( handle == SSL_Socket::invalidSocket() )
        {
            Logger::logItLn("[SSL Server] accept failed");
            return false;
        }
    }
    catch(...)
    {
        Logger::logItLn("[SSL Server] accept failed - Exception");
        return false;
    }
    return true;
}

bool SSL_Server::close()
{
    try
    {
        if( !internal_threads.empty() )
        {
            for(list<SERVER_INTERNAL_THREAD*>::iterator p_trd = internal_threads.begin(); p_trd != internal_threads.end(); ++p_trd)
                (*p_trd)->stop();
            internal_threads.clear();
        }
        ::close(listen_sock);
    }
    catch(...)
    {
        Logger::logItLn("[SSL Server] Close Failed - Exception");
        return false;
    }
    return true;
}

bool SSL_Server::initializeConnection(GraVitoN::Core::SSL_Client &client_sock)
{
    client_sock.loadPemFromFile(cert_file, key_file);
    return client_sock.handshake_accept();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SSL_Server::finalizeConnection(Core::SSL_Client &client_sock)
{
    client_sock.close();
    return true;
}

}
}
#endif
