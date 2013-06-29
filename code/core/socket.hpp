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
            /// is str a valid ipv4 string
            static bool isAValidIPv4(const string &str);

            /// DNS domain lookup (using IPv4)
            static string dnsLookup4(const std::string &domain);

            /// host to network: 32bit
            static unsigned int hostToNet_32(const unsigned int &val);

            /// host to network: 16bit
            static unsigned int hostToNet_16(const unsigned short &val);

            /// network to host: 32 bit
            static unsigned int netToHost_32(const unsigned int &val);

            /// network to host: 16 bit
            static unsigned int netToHost_16(const unsigned short &val);

            /// Convert a hexadecimal IPv4 representation to a string representation
            static string hexToStrIPv4(const unsigned int hex);

            /// Convert a string IPv4 representation to a hexadecimal representation
            static unsigned int strToHexIPv4(string str);
                
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

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Socket::isAValidIPv4(const string &str)
        {
            if( str.size() < 7 )
                return false;
            if( str.size() > 15 )
                return false;

            int dot = 0;
            for(size_t i = 0; i < str.size(); ++i)
                if( str[i] == '.' )
                    ++dot;
                else if( !isdigit(str[i]) )
                    return false;
            return dot == 3;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        string Socket::dnsLookup4(const std::string &domain)
        {
            //for(int i=0; domain[i] != '\0'; ++i)
            //    cout << (int)domain[i] << endl;

            hostent *m_hostent;
            m_hostent = gethostbyname( domain.c_str() );
            if( !m_hostent )
                return "";
            unsigned int addr;
            memcpy((void*)&addr, (void*)*m_hostent->h_addr_list, 4);

            return string(
                inet_ntoa(
                    *(in_addr*)&addr
                    )
                );
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        unsigned int Socket::hostToNet_32(const unsigned int &val)
        {
            return ntohl(val);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        unsigned int Socket::hostToNet_16(const unsigned short &val)
        {
            return htons(val);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        unsigned int Socket::netToHost_32(const unsigned int &val)
       {
            return ntohl(val);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        unsigned int Socket::netToHost_16(const unsigned short &val)
        {
            return ntohs(val);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        string Socket::hexToStrIPv4(const unsigned int hex)
        {
            if(!hex)
                return "";

            char ipc[16];
            unsigned char hip[4];
            memcpy((void*)hip, (void*)&hex, 4);
            snprintf(ipc, sizeof(ipc), "%u.%u.%u.%u",
                     (unsigned int)hip[3],
                     (unsigned int)hip[2],
                     (unsigned int)hip[1],
                     (unsigned int)hip[0]);
            return string(ipc);

        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        unsigned int Socket::strToHexIPv4(string str)
        {
            string res;

            unsigned int hexip;
            unsigned char hex[4];
            sscanf (str.c_str(), "%u.%u.%u.%u",
                    (unsigned int *)&hex[3],
                    (unsigned int *)&hex[2],
                    (unsigned int *)&hex[1],
                    (unsigned int *)&hex[0]);

            memcpy((void*)&hexip, (void*)hex, 4);
            return hexip;
        }
    } /// end of Core
} /// end of GraVitoN

#endif /// end of GRAVITON_SOCKET_H
