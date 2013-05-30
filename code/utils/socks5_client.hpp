#ifndef _GVN_SOCKS5_CLIENT_HEAD_
#define _GVN_SOCKS5_CLIENT_HEAD_

#include <graviton.hpp>
#include <utils/socks5.hpp>
#include <utils/netkit.hpp>
#include <core/tcp_client.hpp>

namespace GraVitoN
{
namespace Utils
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class SOCKS5_Client : public Core::TCP_Client
{
protected:
    unsigned int remote_port;
    string remote_host;
    string username;
    string pass;
    unsigned char cmd;

    bool auth_mod;

protected:
    virtual bool greeting();
    virtual bool authenticate();
    virtual bool askConnect();
    virtual bool askBind();
    virtual bool askUDP();

public:
    /// @param [in] _remote_host
    /// IP or Domain
    SOCKS5_Client(
            const string &_socks_server_ip,
            const unsigned int _port,
            const string _remote_host,
            const unsigned _remote_port,
            const string &_username = "",
            const string &_pass = "",
            const unsigned char _socks_type = SOCKS5::CMD_CONNECT):
        TCP_Client(_socks_server_ip, _port)
    {
        username = _username;
        pass = _pass;
        cmd = _socks_type;

        remote_host = _remote_host;
        remote_port = _remote_port;
    }

    ~SOCKS5_Client()throw()
    {

    }

    virtual bool connect();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Client::connect()
{
    Core::Logger::logItLn("[SOCKS5 Client] Connecting...");
    if( TCP_Client::connect() )
    {
        if( !greeting() )
            return false;

        if( auth_mod )
            if( !authenticate() )
                return false;

        /// @todo bind, udp
        if( cmd == SOCKS5::CMD_CONNECT )
            return askConnect();

        return true;
    }
    return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Client::greeting()
{
    Core::Logger::logItLn("[SOCKS5 Client] Greeting...");

    unsigned char *data = new unsigned char[5];
    size_t sz = 0;

    data[sz++] = SOCKS5::VER;
    data[sz++] = 0x02; // 2 auth methods
    data[sz++] = SOCKS5::AUTH_NONE;
    data[sz++] = SOCKS5::AUTH_USERPASS;

    if( TCP_Client::send(data, sz) )
    {
        if( TCP_Client::recv(data, sz) )
        {
            if( sz <= 1 || data[1] == SOCKS5::REP_AUTH_UNACCEPTABLE )
                return false;
            auth_mod = (data[1] == SOCKS5::AUTH_USERPASS) && (!username.empty() || !pass.empty());
            cout << "AUTH MOD: " << auth_mod << endl;
            Core::Logger::logItLn("[SOCKS5 Client] Done");
            return true;
        }

        return false;
    }
    return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Client::authenticate()
{
    Core::Logger::logItLn("[SOCKS5 Client] Authenticating...");
    unsigned char *data = new unsigned char[3 + username.size() + pass.size() + 1];
    size_t sz = 0;

    data[sz++] = SOCKS5::AUTH_VER;
    data[sz++] = (unsigned char)username.size();
    for(size_t i=0; i<username.size(); ++i)
        data[sz++] = (unsigned char)username[i];
    data[sz++] = (unsigned char)pass.size();
    for(size_t i=0; i<pass.size(); ++i)
        data[sz++] = (unsigned char)pass[i];

    if( TCP_Client::send(data, sz) )
    {
        if( TCP_Client::recv(data, sz) )
        {
            if( sz <= 1 || data[1] != SOCKS5::REP_SUCCEEDED )
            {
                Core::Logger::logItLn("[SOCKS5 Client] FAILED");
                cout << "size: " << sz << ", DATA: " << (int)data[1] << endl;
                return false;
            }
            Core::Logger::logItLn("[SOCKS5 Client] Done");
            return true;
        }
        Core::Logger::logItLn("[SOCKS5 Client] NO ANSWER");
        return false;
    }
    return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool SOCKS5_Client::askConnect()
{
    Core::Logger::logItLn("[SOCKS5 Client] Ask CONNECT...");

    unsigned char *data = new unsigned char[5];
    size_t sz = 0;

    data[sz++] = SOCKS5::VER;
    data[sz++] = SOCKS5::CMD_CONNECT;
    data[sz++] = SOCKS5::RSV;

    /// @todo IPv6
    if( Netkit::isAValidIPv4(remote_host) )
    {
        data[sz++] = SOCKS5::ATYP_IPV4;

        unsigned int hip = Netkit::hostToNet_32( Netkit::strToHexIPv4(remote_host) );
        memcpy((void*)(data+sz), (void*)&hip, 4);
        sz += 4;
    }
    else
    {
        data[sz++] = SOCKS5::ATYP_DOMAINNAME;

        size_t len = remote_host.size();
        data[sz++] = (unsigned char)len;
        memcpy((void*)(data+sz), (void*)remote_host.c_str(), len);
        sz += len;
    }

    unsigned int hport = Netkit::hostToNet_16( remote_port );
    memcpy((void*)(data+sz), (void*)&hport, 2);
    sz += 2;

    if( TCP_Client::send(data, sz) )
    {
        if( TCP_Client::recv(data, sz) )
        {
            if( sz <= 1 || data[1] != SOCKS5::REP_SUCCEEDED )
            {
                Core::Logger::logItLn("[SOCKS5 CLIENT] [FAILD]");
                return false;
            }
            return true;
        }

        return false;
    }
    return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Request BIND
bool SOCKS5_Client::askBind()
{
    Core::Logger::logItLn("[SOCKS5 Client] Ask BIND...");
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Request UDP
bool SOCKS5_Client::askUDP()
{
    Core::Logger::logItLn("[SOCKS5 Client] Ask UDP...");
}

}
}
#endif
