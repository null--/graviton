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
 * @brief GraVitoN::Core::UDP_Socket
 *
 */

#ifndef GRAVITON_UDP_CLIENT_H
#define GRAVITON_UDP_CLIENT_H

#include <graviton.hpp>
#include <core/memory.hpp>
#include <core/logger.hpp>
#include <core/socket.hpp>

namespace GraVitoN
{

    namespace Core
    {
        /// A UDP String Packet
        struct UDP_Packet_String
        {
            std::string     data;
            std::string     ip;
            guint           port;

            UDP_Packet_String(const std::string &data_ = "", const std::string ip_ = "", const guint port_ = 0)
                { data = data_; ip = ip_; port = port_; }
        };

        /// UDP Socket (send/recv/bind)
        class UDP_Socket
        {
        protected:
            /// Socket
            Socket::Handle sock;
            Socket::Address sa;

        public:
            /// Constructor
            UDP_Socket();
            
            virtual ~UDP_Socket();

            virtual bool open();

            virtual bool bind(const std::string &bind_ip, const guint port);
            
            virtual bool close();

            /// Like recv but also stored ip address and port number of sender
            template<class Type>
            bool recv(Memory<Type> &data, string &_sender_ip, guint &_sender_port);

            // virtual string recvString(string &_sender_ip, unsigned int &_sender_port)
            UDP_Packet_String recvString();

            /// Send data to client
            template<class Type>
            bool send(const Memory<Type> &data, const string &ip, const guint &port);

            /// send string
            bool sendString(const UDP_Packet_String &packet);
            
            virtual bool isActive();
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        UDP_Socket::UDP_Socket()
        {
            Socket::getInstance();

            sock = Socket::Invalid;
            // open();
        }

        UDP_Socket::~UDP_Socket()
        {
            if( !Socket::invalidSocket(sock) )
                UDP_Socket::close();
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool UDP_Socket::open()
        {
            try
            {
                UDP_Socket::close();
                
                sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

                if( Socket::invalidSocket( sock ) )
                {
                    Logger::logItLn("[UDP_Socket] socket creation failed");
                    return false;
                }
            }
            catch(...)
            {
                Logger::logItLn("[UDP_Socket] open exception");
                return false;
            }
            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool UDP_Socket::bind(const std::string &local_ip, const guint port)
        {
            sa.sin_family      = AF_INET;
            /// server IP
            sa.sin_addr.s_addr = inet_addr (local_ip.c_str());
            /// Server Port
            sa.sin_port        = htons     (port);
                
            if( Socket::socketError( ::bind(sock, (sockaddr*)&sa, sizeof(sa)) ) )
            {
                Logger::logItLn("[UDP_Socket] bind failed");
                sock = Socket::Invalid;
                return false;
            }

            return true;
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//        
        bool UDP_Socket::close()
        {
            // Logger::logItLn("[UDP_Client] closing...");
            try
            {
                if( !Socket::invalidSocket(sock) )
                {
#if defined(INFO_OS_WINDOWS)
                    ::closesocket(sock);
#else
                    ::close(sock);
#endif
                }
                sock = Socket::Invalid;
            }
            catch(...)
            {
                Logger::logItLn("[UDP_Client] close - EXCEPTION");
                return false;
            }
            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool UDP_Socket::send(const Memory<Type> &data, const string &ip, const unsigned int &port)
        {
            try
            {
                Socket::Address cl_sa;
                memset((char *) &cl_sa, 0, sizeof(cl_sa));
                cl_sa.sin_family = AF_INET;
                cl_sa.sin_port = Socket::hostToNet_16(port);
                if ( inet_aton(ip.c_str(), &cl_sa.sin_addr) == 0 )
                {
                    Logger::logItLn("[UDP Client] inet_aton failed");
                    return false;
                }
                
                int err = ::sendto(sock, (char*)data.address(), data.size(), 0, (sockaddr*)&cl_sa, sizeof(cl_sa));
                if( Socket::socketError( err ) )
                {
                    Logger::logItLn("[UDP Client] send failed");
                    return false;
                }
            }
            catch(...)
            {
                Logger::logItLn("[UDP_Socket] send exception");
                return false;
            }

            return true;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool UDP_Socket::sendString(const UDP_Packet_String &packet)
        {
            Memory<char> data = packet.data;
            return send(data, packet.ip, packet.port);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
            bool UDP_Socket::recv(Memory<Type> &data, string &ip, guint &port)
        {
            try
            {
                Socket::Address cl_sa;

                guint sz = sizeof(Socket::Address);
                Type buf[ Config::MAX_TCP_PACKET_SIZE + 1];
                int err = ::recvfrom(sock, buf, Config::MAX_TCP_PACKET_SIZE, 0, (sockaddr*)&cl_sa, &sz);

                if( Socket::socketError( err ) )
                {
                    Core::Logger::logItLn("[UDP Socket] recv failed");
                    return false;
                }

                data.copy(buf, err);

                char *addr = inet_ntoa(cl_sa.sin_addr);
                if( !addr )
                    ip = "";
                else
                    ip = addr;
                port = Socket::netToHost_16(cl_sa.sin_port);
            }
            catch(...)
            {
                Logger::logItLn("[UDP_Socket] recv exception");
                return false;
            }

            return true;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        UDP_Packet_String UDP_Socket::recvString()
        {
            UDP_Packet_String pack;
            Memory<char> data;

            pack.data = "";
            if( UDP_Socket::recv(data, pack.ip, pack.port) )
            {
                pack.data = data.toString();
            }

            return pack;
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool UDP_Socket::isActive()
        {
            return !Socket::invalidSocket( sock );
        }

    } // Core

//=============================================================================//
#ifdef GVN_ACTIVATE_LUABRIDGE
    namespace LUABridge
    {

        void addClass_UDP_Socket()
        {
            Core::Logger::logItLn("Loaing GraVitoN.Core.UDP_Socket");

            luabridge::getGlobalNamespace ( Core::Luaviton::getInstance().getState() )
                .beginNamespace("GraVitoN")
                .beginNamespace("Core")
                .beginClass <Core::UDP_Socket> ("UDP_Socket")
                .addConstructor < void(*) (), RefCountedPtr<Core::UDP_Socket> > ()
                .addConstructor < void(*) (const unsigned int&), RefCountedPtr<Core::UDP_Socket> > ()
                // .addConstructor < void(*) (Core::TCP_Socket), RefCountedPtr<Core::UDP_Socket> > ()
                .addFunction("open", &Core::UDP_Socket::open)
                .addFunction("close", &Core::UDP_Socket::close)
                .addFunction("sendString", &Core::UDP_Socket::sendString)
                .addFunction("recvString", &Core::UDP_Socket::recvString)
                .addFunction("isActive", &Core::UDP_Socket::isActive)
                .endClass()
                .endNamespace()
                .endNamespace()
                ;
        }
    }
#endif

} // end of GraVitoN

#endif // GRAVITON_UDP_CLIENT
