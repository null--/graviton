/// @todo socks5 server
#ifndef _GVN_SOCKS5_SRV_HEAD_
#define _GVN_SOCKS5_SRV_HEAD_

#include <utils/socks5.hpp>
#include <utils/network_relay.hpp>
#include <core/tcp_server.hpp>
#include <core/tcp_client.hpp>
#include <core/udp_client.hpp>
namespace GraVitoN
{

namespace Utils
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class SOCKS5_Server : public Core::TCP_Server
{
protected:
    bool auth;
    string user;
    string pass;

protected:
    bool authenticate();
    void reply(Core::TCP_Client &client_sock, const unsigned char reply_code);

    bool cmdConnect(
            Core::TCP_Client &client_sock,
            const string &remote_ip,
            const unsigned int &port);

    bool cmdBind(
            Core::TCP_Client &client_sock,
            const string &remote_ip,
            const unsigned int &remote_port,
            const unsigned int &client_port);

    bool cmdUDP(
            Core::TCP_Client &client_sock,
            const string &remote_ip,
            const unsigned int &port);

    bool response(Core::TCP_Client &client_sock);

public:
    SOCKS5_Server(
            unsigned int local_port,
            const bool &auth_mod = false,
            const string &username = "",
            const string &password = "");

    virtual ~SOCKS5_Server()throw();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
SOCKS5_Server::SOCKS5_Server(unsigned int local_port, const bool &auth_mod, const string &username, const string &password)
    :TCP_Server(local_port)
{
    auth = auth_mod;
    user = username;
    pass = password;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
SOCKS5_Server::~SOCKS5_Server()throw()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
void SOCKS5_Server::reply(Core::TCP_Client &client_sock, const unsigned char reply_code)
{

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Server::cmdConnect(Core::TCP_Client &client_sock, const string &remote_ip, const unsigned int &port)
{

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Server::cmdBind(Core::TCP_Client &client_sock, const string &remote_ip, const unsigned int &remote_port, const unsigned int &client_port)
{

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Server::cmdUDP(Core::TCP_Client &client_sock, const string &remote_ip, const unsigned int &port)
{

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Server::authenticate()
{

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Server::response(Core::TCP_Client &client_sock)
{
    return true;
}

}
}
#endif
