#ifndef _GVN_NETWORK_RELAY_HEAD_
#define _GVN_NETWORK_RELAY_HEAD_

#include <graviton.hpp>
#include <core/tcp_client.hpp>
#include <core/tcp_server.hpp>
#include <core/udp_client.hpp>

namespace GraVitoN
{

namespace Utils
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// bob --> [relay server] --> alice
/// @todo Network_Relay_Backward
class Network_Relay_Tcp : public Core::TCP_Server
{
protected:
    string left_ip;
    string right_ip;
    unsigned int right_remote_port;

protected:
    /// left --> [relay server] --> right
    /// left(recv), right(send), right(recv), left(send)
    virtual bool response(Core::TCP_Client &left)
    {
        Core::Logger::logItLn("Relaying...");

        //if( left_ip.size() != 0 && left.getRemoteIP() != left_ip )
            //return false;

        Core::TCP_Client right(right_ip, right_remote_port);

        Core::Logger::logItLn("[Relay] connecting...");
        right.connect();

        unsigned char *data;
        size_t data_size;

        while( right.isActive() )
        {
            try
            {
                // left.send((unsigned char*)"hell", 5);
                // right.send((unsigned char*)"hell", 5);
                // break;
                Core::Logger::logItLn("[Relay] recv left...");
                left.recv(data, data_size);
                Core::Logger::logItLn("[Relay] send right...");
                right.send(data,data_size);
                Core::Logger::logItLn("[Relay] recv right...");
                right.recv(data, data_size);
                Core::Logger::logItLn("[Relay] send left...");
                left.send(data, data_size);
            }
            catch(ting::net::Exc &e)
            {
                Core::Logger::logVariable("Network error", e.What());
                return false;
            }
        }

        return true;
    }

public:
    Network_Relay_Tcp(
            const string &          client_ip,
            const unsigned int &    client_port,
            const string &          server_ip,
            const unsigned int &    server_port,
            const bool              multi_thread = true):
        TCP_Server(client_port, multi_thread)
    {
        right_ip = server_ip;
        left_ip = client_ip;
        right_remote_port = server_port;
    }

    ~Network_Relay_Tcp()throw()
    {
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Network_Relay_Bidirectional_TCP
/// bob <--> [relay server] <--> alice
class Network_Relay_Bidirectional_TCP
{

};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Network_Relay_Udp
class Network_Relay_Udp
{

};

}

}
#endif
