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
 * @brief GraVitoN::Core::Socket
*/

#ifndef GRAVITON_SOCKET_H
#define GRAVITON_SOCKET_H

#include <graviton.hpp>
#include <core/component.hpp>

#if defined(INFO_OS_WINDOWS)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <stdint.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

namespace GraVitoN
{
    namespace Core
    {
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        class Socket : public Component_Singleton<Socket> 
        {
        public:
#ifdef INFO_OS_WINDOWS
            typedef SOCKET Handle;
            typedef int Size;
#else
            typedef int Handle;
            typedef socklen_t Size;
#endif

            typedef struct sockaddr_in Address;

#ifdef INFO_OS_WINDOWS
            const static Handle Invalid = INVALID_SOCKET;
            const static int Error = SOCKET_ERROR;
#else
            const static Handle Invalid = -1;
            const static int Error = -1;
#endif
            /// check if socket is invalid
            inline static bool invalidSocket(Handle socket_)
                { return socket_ == Socket::Invalid; }

            /// check if there is a socket error
            inline static bool socketError(int error_)
                { return error_ == Socket::Error; }

        public:
            Socket();
            ~Socket();
        };

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Socket::Socket()
        {
#if defined(INFO_OS_WINDOWS)
            WSADATA data;
            int res = WSAStartup(MAKEWORD(2, 2), &data);

            if( res != 0 )
            {
                throw( std::logic_error("[Socket] WSAStartup Failed") );
            }
#endif
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Socket::~Socket()
        {
            /// NO NEED FOR WSACleanUp
        }
    } /// end of Core
} /// end of GraVitoN

#endif /// end of GRAVITON_SOCKET_H
