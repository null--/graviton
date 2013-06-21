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
 * @brief GraVitoN::Core::SSL_Socket
 */

#ifndef GRAVITON_SSL_SOCKET_H
#define GRAVITON_SSL_SOCKET_H

#include <graviton.hpp>
#include <core/socket.hpp>
#include <core/logger.hpp>
#ifdef INFO_OS_WINDOWS
    #include <external/openssl/ms/applink.c>  // To prevent crashing (see the OpenSSL FAQ)
#endif

#include <external/openssl/include/openssl/bio.h> // BIO objects for I/O
#include <external/openssl/include/openssl/ssl.h> // SSL and SSL_CTX for SSL connections
#include <external/openssl/include/openssl/err.h> // Error reporting
	
namespace GraVitoN
{
    namespace Core
    {
        class SSL_Socket : public Component_Singleton<SSL_Socket>
        {
        public:
            SSL_Socket();
        };

        SSL_Socket::SSL_Socket()
        {
            Socket::getInstance();
            
            SSL_load_error_strings();
            SSL_library_init();
            SSLeay_add_ssl_algorithms();
            OpenSSL_add_all_algorithms();
        }
    }
}
#endif
