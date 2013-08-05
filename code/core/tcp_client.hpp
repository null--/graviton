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
 * @brief GraVitoN::Core::TCP_Client
 */

#ifndef GRAVITON_TCP_CLIENT_H
#define GRAVITON_TCP_CLIENT_H

#include <graviton.hpp>
#include <core/socket.hpp>
#include <core/memory.hpp>

namespace GraVitoN
{
    namespace Core
    {

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        class TCP_Client
        {
        protected:
            /// is socket open
            bool is_dead;
            /// socket handle
            Socket::Handle sock;

            /// remote ip address (string)
            std::string r_ip_str;
            /// local ip address
            std::string l_ip_str;
            
            /// remote ip address (hex)
            guint r_ip_hex;
            /// local ip hex
            guint l_ip_hex;
            
            /// remote port
            guint r_port;
            /// local port
            guint l_port;
            
            /// socket address
            Socket::Address sa;
            
        public:
            TCP_Client(const string &ip, const guint port);
            TCP_Client(Socket::Handle _sock, Socket::Address _sa);
        
            virtual ~TCP_Client();

            /// connect to server
            virtual bool connect();

            /// close socket
            virtual bool close();

            /// recieve data from server
            template<class Type>
            bool recv(Memory<Type> &data);

            /// recv string from server
            string recvString();

            /// send data to server
            template<class Type>
            bool send(const Memory<Type> &data);

            /// send string to server
            bool sendString(const std::string &data);

            /// is connection still alive
            virtual bool isActive();

            /// get local port
            guint getLocalPort()
                { return l_port; }

            /// get local ip address (string)
            std::string getLocalIP()
                { return l_ip_str; }

            /// get local ip address (hex)
            guint getLocalIPHex()
                { return l_ip_hex; }

            /// get server ip address (string)
            std::string getRemoteIP()
                { return r_ip_str; }

            /// get server ip address (hex)
            guint getRemoteIPHex()
                { return r_ip_hex; }

            /// get server port number
            guint getRemotePort()
                { return r_port; }

            /// get socket handle (DANGEROUS)
            Socket::Handle getHandle()
                { return sock; }
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        TCP_Client::TCP_Client(const string &ip, const guint port)
        {
            is_dead = true;

            Socket::getInstance();
            r_ip_str = ip;
            r_port = port;

            memset (&sa, '\0', sizeof(sa));
            sa.sin_family      = AF_INET;
            /// server IP
            sa.sin_addr.s_addr = inet_addr (ip.c_str());
            /// Server Port
            sa.sin_port        = htons     (r_port);

            sock = Socket::Invalid;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
    TCP_Client::TCP_Client(Socket::Handle _sock, Socket::Address _sa)
        {
            is_dead = Socket::invalidSocket(_sock);

            sock = _sock;
            sa = _sa;

            r_port = ntohs( sa.sin_port );
            r_ip_hex = ntohl( sa.sin_addr.s_addr );
            r_ip_str = inet_ntoa( sa.sin_addr );
            
            Socket::Address sin;
            socklen_t len = sizeof(sin);
            if( getsockname(sock, (struct sockaddr*)&sin, &len) == Socket::Error )
            {
                l_port = 0;
                l_ip_hex = 0;
                l_ip_str = "";
            }
            else
            {
                l_port = ntohs( sin.sin_port );
                l_ip_hex = ntohl( sin.sin_addr.s_addr );
                l_ip_str = inet_ntoa( sin.sin_addr );
            }
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        TCP_Client::~TCP_Client()
        {
        }


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool TCP_Client::connect()
        {
            is_dead = true;
            Logger::logItLn("[TCP_Client] connecting");
//            try
//            {
                /* ----------------------------------------------- */
                /* Create a socket and connect to server using normal socket calls. */

                sock = socket (AF_INET, SOCK_STREAM, 0);

                int err = ::connect(sock, (struct sockaddr*) &sa, sizeof(sa));

                if( Socket::socketError(err) )
                {
                    Logger::logItLn("[TCP_Client] connect failed");
                    return false;
                }
//            }
//            catch(...)
//            {
//                Logger::logItLn("[TCP_Client] connect failed - EXCEPTION");
//                return false;
//            }

            Socket::Address sin;
            socklen_t len = sizeof(sin);
            if( getsockname(sock, (struct sockaddr*)&sin, &len) == Socket::Error )
            {
                l_port = 0;
                l_ip_hex = 0;
                l_ip_str = "";
            }
            else
            {
                l_port = ntohs( sin.sin_port );
                l_ip_hex = ntohl( sa.sin_addr.s_addr );
                l_ip_str = inet_ntoa( sa.sin_addr );
            }
            
            is_dead = false;
            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool TCP_Client::close()
        {
            Logger::logItLn("[TCP_Client] closing...");
            try
            {
                is_dead = true;
                /* Clean up. */
                if( !Socket::invalidSocket(sock) )
                {
#if defined(INFO_OS_WINDOWS)
                    ::closesocket(sock);
#else
                    ::close(sock);
#endif
                }

            }
            catch(...)
            {
                Logger::logItLn("[TCP_Client] close - EXCEPTION");
                return false;
            }
            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool TCP_Client::send(const Memory<Type> &data)
        {
            // cout << "damn send..." << endl;
            if( !isActive() )
                return false;

            // cout << "a lost try..." << endl;
            //try
            //{
            int err = ::send(sock, (char*)data.address(), data.size(), 0);
            // cout << "::send..." << endl;
            if( Socket::socketError(err) )
            {
                Logger::logVariable("[TCP_Client] send failed err", err);
                // close();
                return false;
            }
            // Logger::logVariable("[TCP_Client] send success", err);
            //}
            //catch(...)
            //{
            // Logger::logItLn("[TCP_Client] send failed - EXCEPTION");
            //return false;
            //}
            return true;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool TCP_Client::sendString(const std::string &data)
        {
            Memory<char> data_ = data;
            return TCP_Client::send(data_);
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool TCP_Client::recv(Memory<Type> &data)
        {
            if( sock == Socket::Invalid )
                return false;
//            try
//            {
            // Logger::logVariable("[TCP_Client] recving", ssl);
            Type buf[ Config::MAX_TCP_PACKET_SIZE + 1];
            int err = -1;

            err = ::recv(sock, (char*)buf, Config::MAX_TCP_PACKET_SIZE * sizeof(guchar), 0);
                
            if( Socket::socketError(err) || err == 0)
            {
                close();
                Logger::logVariable("[TCP_Client] recv failed", err);
                return false;
            }
            // Logger::logVariable("[TCP_Client] recv", err);

            // buf[ err ] = '\0';
            // data.copy(buf, err + 1);
            data.copy(buf, err);
//            }
//            catch(...)
//            {
//                Logger::logItLn("[TCP_Client] recv failed - EXCEPTION");
//                return false;
//            }
            return true;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        std::string TCP_Client::recvString()
        {
            // sdata = "";
            Memory<char> data;
            if( TCP_Client::recv(data) )
            {
                return data.toString();
            }

            return "";
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool TCP_Client::isActive()
        {
            return !is_dead && !Socket::invalidSocket(sock);
        }

    }

}
#endif
