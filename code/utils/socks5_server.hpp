/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
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
 * @brief GraVitoN socks5 server
 *
 */
#ifndef _GVN_SOCKS5_SRV_HEAD_
#define _GVN_SOCKS5_SRV_HEAD_

#include <utils/socks5.hpp>
#include <utils/network_relay.hpp>
#include <core/memory.hpp>
#include <core/tcp_server.hpp>
#include <core/tcp_client.hpp>
#include <core/udp_socket.hpp>
namespace GraVitoN
{

    namespace Utils
    {

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo IPv6 support
        class SOCKS5_Server : public Core::TCP_Server
        {
        protected:
            string user;
            string pass;

            unsigned char ver;
            unsigned char cmd;
            unsigned char atyp;
            unsigned int dstport;
            string dstip;

        protected:
            bool authenticate(Core::TCP_Client &client_sock);
            void reply(Core::TCP_Client &client_sock, const unsigned char reply_code);

            bool cmdConnect(
                Core::TCP_Client &client_sock,
                const string &remote_ip,
                const unsigned int &remote_port);

            bool cmdBind(
                Core::TCP_Client &client_sock,
                const string &remote_ip,
                const unsigned int &client_port);

            bool cmdUDP(
                Core::TCP_Client &client_sock,
                const string &remote_ip,
                const unsigned int &remote_port);

            static int processRequest(
                const Core::Memory<guchar> &data,
                unsigned char &ver,
                unsigned char &cmd,
                unsigned char &atyp,
                string &dstip,
                unsigned int &dstport);

            bool greeting(Core::TCP_Client &client_sock);
            bool initializeConnection(Core::TCP_Client &client_sock);
            bool response(Core::TCP_Client &client_sock);

        public:
            SOCKS5_Server(
                const string &local_ip,
                unsigned int local_port,
                const string &username = "",
                const string &password = "");

            virtual ~SOCKS5_Server()throw();
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        SOCKS5_Server::SOCKS5_Server(const string &local_ip, unsigned int local_port, const string &username, const string &password)
        :TCP_Server(local_ip, local_port, true)
        {
            user = username;
            pass = password;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        SOCKS5_Server::~SOCKS5_Server()throw()
        {
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        int SOCKS5_Server::processRequest(
            const Core::Memory<guchar> &data,
            unsigned char &ver,
            unsigned char &cmd,
            unsigned char &atyp,
            string &dstip,
            unsigned int &dstport)
        {
            Core::Logger::logItLn("[SOCKS5] Processing Packet");
            if( data.size() < 4 )
                return SOCKS5::REP_COMMAND_NOT_SUPPORTED;

            ver = (char)( data[0] );
            // Core::Logger::logVariable("ver",(int)ver);
            cmd = (char)( data[1] );
            // Core::Logger::logVariable("cmd",(int)cmd);
            atyp = (char)( data[3]);
            // Core::Logger::logVariable("atyp",(int)atyp);

            if( ver != SOCKS5::VER )
                return SOCKS5::REP_COMMAND_NOT_SUPPORTED;

            /// @todo SOCKS5::CMD_BIND
            if( cmd != SOCKS5::CMD_UDP &&
                cmd != SOCKS5::CMD_BIND &&
                cmd != SOCKS5::CMD_CONNECT )
                return SOCKS5::REP_COMMAND_NOT_SUPPORTED;

            /// @todo SOCKS5::ATYP_IPV6
            if( atyp != SOCKS5::ATYP_IPV4 &&
                atyp != SOCKS5::ATYP_DOMAINNAME )
                return SOCKS5::REP_COMMAND_NOT_SUPPORTED;

            if( atyp == SOCKS5::ATYP_IPV4 )
            {
                if( data.size() < 10 )
                    return SOCKS5::REP_COMMAND_NOT_SUPPORTED;

                unsigned int hip, hport = 0;
                memcpy((void*)&hip, (void*)(data + 4), 4);
                memcpy((void*)&hport, (void*)( data + data.size() - 2 ), 2);

                /// Bloody ntohl
                hip = Core::Socket::netToHost_32(hip);
                dstport = Core::Socket::netToHost_16(hport);
                dstip = Core::Socket::hexToStrIPv4(hip);
            }
            else if( atyp == SOCKS5::ATYP_DOMAINNAME )
            {
                unsigned int hport = 0, domsz = data[4];
                char domain[ data.size() ];
                /// data + 4 + 1 for size of domain
                memcpy((void*)&domain, (void*)(data+5), domsz);
                memcpy((void*)&hport, (void*)(data + data.size() - 2), 2);

                domain[domsz] = '\0';

                Core::Logger::logVariable("[SOCKS5] Domain",(char*)domain);
                // Core::Logger::logVariable("Domain Len",string((char*)domain).size());
                dstip = Core::Socket::dnsLookup4(domain);
                dstport = Core::Socket::netToHost_16(hport);
                if( dstip.size() == 0 )
                    return SOCKS5::REP_HOST_UNREACHABLE;
            }

            Core::Logger::logVariable("[SOCKS5] IP", dstip);
            Core::Logger::logVariable("[SOCKS5] Port", dstport);

            return SOCKS5::REP_SUCCEEDED;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool SOCKS5_Server::cmdConnect(Core::TCP_Client &client_sock, const string &remote_ip, const unsigned int &remote_port)
        {
            Core::Logger::logItLn("[SOCKS5] Connecting...");

            unsigned int bind_port = port;
            unsigned int nhbp = Core::Socket::hostToNet_16(bind_port);
            unsigned int lip = Core::Socket::hostToNet_32( client_sock.getLocalIPHex() );

            /// @todo IPv6
            size_t repdata_size = 1 + 1 + 1 + 1 + 4 + 2;
            Core::Memory<guchar> repdata(repdata_size);
            repdata[0] = SOCKS5::VER;
            repdata[1] = SOCKS5::REP_SUCCEEDED;
            repdata[2] = SOCKS5::RSV;
            repdata[3] = SOCKS5::ATYP_IPV4;

            repdata.copy((guchar*)&lip, 4, 4);
            repdata.copy((guchar*)&nhbp, 2, 8);
            // memcpy( (void*)(repdata+4), (void*)&lip, 4);
            // memcpy( (void*)(repdata+8), (void*)&(nhbp), 2);

            Core::TCP_Client rhost(remote_ip, remote_port);
            /*
              if( !rhost.connect() )
              {
              repdata[1] = SOCKS5::REP_CONNECTION_REFUSED;
              client_sock.send(repdata, repdata_size);
              return false;
              }
            */

            Network_Relay_TCP rel(client_sock, rhost);
            if( client_sock.send(repdata) )
                rel.run();
            else
                return false;

            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo cmdBind implement (using Network_Relay_TCP_Server)
        bool SOCKS5_Server::cmdBind(Core::TCP_Client &client_sock, const string &remote_ip, const unsigned int &client_port)
        {
            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo cmdUDP test
        bool SOCKS5_Server::cmdUDP(Core::TCP_Client &client_sock, const string &remote_ip, const unsigned int &remote_port)
        {
            Core::Logger::logItLn("[SOCKS5] UDP Connecting...");

            /// @todo find a random closed port
            unsigned int bind_port = port;
            unsigned int nhbp = Core::Socket::hostToNet_16(bind_port);
            unsigned int lip = Core::Socket::hostToNet_32(client_sock.getLocalIPHex());

            /// @todo IPv6, DomainName
            size_t repdata_size = 1 + 1 + 1 + 1 + 4 + 2;
            // unsigned char repdata[repdata_size];
            Core::Memory<unsigned char> repdata( repdata_size );
            repdata[0] = SOCKS5::VER;
            repdata[1] = SOCKS5::REP_SUCCEEDED;
            repdata[2] = SOCKS5::RSV;
            repdata[3] = SOCKS5::ATYP_IPV4;

            repdata.copy((guchar*)&lip, 4, 4);
            repdata.copy((guchar*)&nhbp, 2, 8);
            // memcpy( (void*)(repdata+4), (void*)&lip, 4);
            // memcpy( (void*)(repdata+8), (void*)&(nhbp), 2);

            Network_Relay_TCP_To_UDP rel(client_sock, bind_ip, remote_ip, remote_port);
            if( client_sock.send(repdata) )
                rel.run();
            else
                return false;

            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool SOCKS5_Server::authenticate(Core::TCP_Client &client_sock)
        {
            Core::Memory<guchar> data;
            
            Core::Logger::logItLn("[SOCKS5] Authenticating...");

            if( !client_sock.recv(data) )
                return false;

            if( data[0] != SOCKS5::AUTH_VER )
            {
                // Core::Logger::logItLn("BAD VER");
                return false;
            }

            string usr, pss;
            size_t len = data[1];
            if( 2 + len > data.size() )
            {
                // Core::Logger::logItLn("BAD USER");
                return false;
            }
            for(size_t i = 0; i < len; ++i)
                usr += data[2 + i];

            size_t plen = data[2+len];
            if( len + plen + 3 > data.size() )
            {
                // Core::Logger::logItLn("BAD PASS");
                return false;
            }

            for(size_t i = 0; i < plen; ++i)
                pss += data[3 + len + i];

            // Core::Logger::logVariable("[SOCKS5] Username", usr);
            // Core::Logger::logVariable("[SOCKS5] Password", pss);
            bool res = (user == usr) && (pass == pss);
            // Core::Logger::logVariable("Result",res);
            //unsigned char repdata[2];
            Core::Memory<guchar> repdata(2);
            
            repdata[0] = SOCKS5::AUTH_VER;
            repdata[1] = (res) ? (SOCKS5::REP_SUCCEEDED) : (SOCKS5::REP_AUTH_UNACCEPTABLE);

            return res && client_sock.send(repdata);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool SOCKS5_Server::greeting(Core::TCP_Client &client_sock)
        {
            Core::Memory<guchar> data;
            
            Core::Logger::logItLn("[SOCKS5] Greeting...");

            if( !client_sock.recv(data) )
                return false;

            if(data.size() < 3)
                return false;

            if(data[0] != SOCKS5::VER)
                return false;

            size_t nauth = data[1];

            unsigned char auth_mod = SOCKS5::REP_AUTH_UNACCEPTABLE;

            bool doauth = user.size() > 0;
            for(size_t i = 0; i < nauth && (2+i) < data.size(); ++i)
            {
                if( doauth )
                {
                    if( data[2+i] == SOCKS5::AUTH_USERPASS )
                        auth_mod = SOCKS5::AUTH_USERPASS;
                }
                else if( data[2+i] == SOCKS5::AUTH_NONE)
                    auth_mod = SOCKS5::AUTH_NONE;
            }

            Core::Memory<guchar> repdata(2);
            repdata[0] = SOCKS5::VER;
            repdata[1] = auth_mod;

            if( !client_sock.send(repdata) )
                return false;

            if( auth_mod == SOCKS5::AUTH_USERPASS )
                return authenticate(client_sock);

            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool SOCKS5_Server::initializeConnection(Core::TCP_Client &client_sock)
        {
            Core::Memory<guchar> data;
            Core::Logger::logItLn("[SOCKS5] New Connection");

            int res;

            /// Greeting and Authentication
            if( !greeting(client_sock) )
            {
                Core::Logger::logItLn("[SOCKS5] [FAILED]");
                client_sock.close();
                return false;
            }

            /// Initializing Tunnel
            if( !client_sock.recv(data) )
                return false;

            res = processRequest(data, ver, cmd, atyp, dstip, dstport);

            if( res )
            {
                Core::Logger::logItLn("[SOCKS5] [FAILED]");
                client_sock.close();
                return false;
            }

            return true;
        }

        bool SOCKS5_Server::response(Core::TCP_Client &client_sock)
        {
            /// CONNECT
            if( cmd == SOCKS5::CMD_CONNECT )
            {
                if( !cmdConnect(client_sock, dstip, dstport) )
                    return false;
                client_sock.close();
            }
            /// BIND
            else if( cmd == SOCKS5::CMD_BIND )
            {
                if( !cmdBind(client_sock, dstip, dstport) )
                    return false;
            }
            /// UDP
            else if( cmd == SOCKS5::CMD_UDP )
            {
                if( !cmdUDP(client_sock, dstip, dstport) )
                    return false;
            }
        }

    }
}
#endif
