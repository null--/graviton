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
/// @todo need more test
class Network_Relay_Tcp : public Core::TCP_Server
{
protected:
    string left_ip;
    string right_ip;
    unsigned int right_remote_port;

    class RELAY_THREAD: public Core::Thread
    {
    protected:
        Core::TCP_Client &sock1;
        Core::TCP_Client &sock2;
        Core::Semaphore &sema;

        virtual bool myMainLoop()
        {
            Core::Logger::logItLn("[Relay Thread] entering relay loop...");
            while( sock1.isActive()  )
            {
                try
                {
                    unsigned char *data = _null_;
                    size_t data_size;

                    // sema.wait();
                    Core::Logger::logItLn("[Relay Thread] recv");
                    if( sock1.recv(data, data_size) )
                    {
                        Core::Logger::logItLn("[Relay Thread] send");

                        sema.wait();
                        bool res = sock2.send(data, data_size);
                        sema.signal();

                        if( !res )
                        {
                            break;
                        }
                    }
                    // sema.signal();
                }
                catch(Thread::Exception &e)
                {
                    // sema.signal();
                    Core::Logger::logVariable("[Relay Thread] Network error", e.What());
                    return false;
                }
            }
            return false;
        }

    public:
        RELAY_THREAD(
                Core::TCP_Client &p_sock1,
                Core::TCP_Client &p_sock2,
                Core::Semaphore &p_sema
                ):
            sock1(p_sock1),
            sock2(p_sock2),
            sema(p_sema)
        {

        }
        ~RELAY_THREAD()throw()
        {

        }
    };

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
        if( !right.connect() )
            return false;

        Core::Logger::logItLn("[Relay] starting listening thread...");
        Core::Semaphore sema;

        // RELAY_THREAD l_trd(left, right, sema);
        RELAY_THREAD r_trd(right, left, sema);

        // l_trd.run();
        r_trd.run();

        unsigned char *data = _null_;
        size_t data_size;

        Core::Logger::logItLn("[Relay] entering relay loop...");
        sema.signal();
        while( left.isActive() )
        {

            if( left.recv(data, data_size) )
            {
                sema.wait();
                if( !right.send(data, data_size) )
                {
                    break;
                }
                sema.signal();
            }
        }
        sema.signal();

        r_trd.stop();

        left.close();
        right.close();

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
/// @todo Network_Relay_Udp
class Network_Relay_Udp
{

};

}

}
#endif
