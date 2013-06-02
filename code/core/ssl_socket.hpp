#ifndef _GVN_SSL_SOCKET_HEAD_
#define _GVN_SSL_SOCKET_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <utils/netkit.hpp>
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
namespace Core
{

class SSL_Socket : public Core::Component_Singleton<SSL_Socket>
{
private:
#ifdef INFO_OS_WINDOWS
    WSADATA wsadata;
#endif

public:
    SSL_Socket()
    {
#ifdef INFO_OS_WINDOWS
        int ret = WSAStartup(0x101, &wsadata);
        if (ret != 0)
        {
            Core::Logger::logVariable("WSAStartup() failed with", GetLastError());
            return;
        }
#endif
        SSL_load_error_strings();
        SSL_library_init();
        SSLeay_add_ssl_algorithms();
        OpenSSL_add_all_algorithms();
    }

#ifdef INFO_OS_WINDOWS
    typedef SOCKET Handle;
    typedef size_t Size;
#else
    typedef int Handle;
    typedef socklen_t Size;
#endif

    typedef struct sockaddr_in Address;

#ifdef INFO_OS_WINDOWS
    inline static Handle invalidSocket(){
        return INVALID_SOCKET;
    }

    inline static int socketError(){
        return SOCKET_ERROR;
    }

#else
    inline static Handle invalidSocket(){
        return -1;
    }

    inline static Handle socketError(){
        return -1;
    }
#endif
};

}
}
#endif
