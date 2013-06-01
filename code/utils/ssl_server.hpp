#ifndef _GVN_SSL_SERVER_HEAD_
#define _GVN_SSL_SERVER_HEAD_

#include <graviton.hpp>
#include <core/tcp_server.hpp>

#ifdef INFO_OS_WINDOWS
    #include <external/openssl/ms/applink.c>  // To prevent crashing (see the OpenSSL FAQ)
#endif
#include <external/openssl/include/openssl/bio.h> // BIO objects for I/O
#include <external/openssl/include/openssl/ssl.h> // SSL and SSL_CTX for SSL connections
#include <external/openssl/include/openssl/err.h> // Error reporting

#include <stdio.h> // If you don't know what this is for stop reading now.

#ifdef INFO_OS_WINDOWS
    #include <winsock2.h> // To prevent crashing (see the OpenSSL FAQ)
#endif

namespace GraVitoN
{
namespace Utils
{

/// @todo SSL Server
class SSL_Server : public Core::TCP_Server
{
protected:
    virtual bool initializeConnection(GraVitoN::Core::TCP_Client &client_sock);
    virtual bool response(GraVitoN::Core::TCP_Client &client_sock);
    virtual bool finalizeConnection(GraVitoN::Core::TCP_Client &client_sock);

public:
    SSL_Server(const unsigned int &port, const bool multi_thread = true):
        TCP_Server(port, multi_thread)
    {
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();
    }
};

bool SSL_Server::initializeConnection(GraVitoN::Core::TCP_Client &client_sock)
{
    try
    {
        typeof(client_sock.getTCPSocket().getSocketHandle())
                client =
                    client_sock.getTCPSocket().getSocketHandle();
        return true;
    }
    catch(Core::TCP_Socket::Exception &e)
    {
        Core::Logger::logVariable("[SSL Server Init] Network error", e.What());
    }

    return false;
}

bool SSL_Server::response(Core::TCP_Client &client_sock)
{

}

bool SSL_Server::finalizeConnection(Core::TCP_Client &client_sock)
{

}
}
}
#endif
