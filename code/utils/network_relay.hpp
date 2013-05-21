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
class Network_Relay_Tcp : public Core::Component
{
private:
    Core::TCP_Client &left;
    Core::TCP_Client &right;

protected:
    virtual void relay()
    {
        Core::Logger::logItLn("Relaying...");

        Core::Logger::logItLn("[Relay] starting listening thread...");

        unsigned char *data = _null_;
        size_t data_size;

        Core::Logger::logItLn("[Relay] entering relay loop...");
        // while( left.isActive() )
        while ( left.isActive() && right.isActive() )
        {
            Core::Logger::logItLn("[Relay] left recv");
            if( left.isActive() && !left.recv(data, data_size) ) break;
            Core::Logger::logItLn("[Relay] right send");
            if( right.isActive() && !right.send(data, data_size) ) break;
            Core::Logger::logItLn("[Relay] right recv");
            if( right.isActive() && !right.recv(data, data_size) ) break;
            Core::Logger::logItLn("[Relay] left send");
            if( left.isActive() && !left.send(data, data_size) ) break;
        }

        Core::Thread::sleep(10);
        if( left.isActive() ) left.close();
        if( right.isActive() ) right.close();
    }

public:
    Network_Relay_Tcp(Core::TCP_Client &_left, Core::TCP_Client &_right):
        left(_left),
        right(_right)
    {

    }

    virtual bool run()
    {
        relay();
        return true;
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo design is not GEEK!
class Network_Relay_Tcp_Bidirectional : public Core::Component
{
private:
    Core::TCP_Client &left;
    Core::TCP_Client &right;

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
            Core::Logger::logItLn("[Relay Thread] Done");
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
    virtual void relay()
    {
        Core::Logger::logItLn("Relaying...");

        Core::Logger::logItLn("[Relay] starting listening thread...");
        Core::Semaphore sema;

        RELAY_THREAD l_trd(left, right, sema);
        RELAY_THREAD r_trd(right, left, sema);

        l_trd.run();
        r_trd.run();

        // unsigned char *data = _null_;
        // size_t data_size;

        Core::Logger::logItLn("[Relay] entering relay loop...");
        sema.signal();
        // while( left.isActive() )
        while ( r_trd.isActive() && l_trd.isActive() )
        {
            Core::Thread::sleep(10);

            /*
            if( left.isActive() && left.recv(data, data_size) )
            {
                sema.wait();
                if( !right.send(data, data_size) )
                {
                    break;
                }
                sema.signal();
            }

            if(!r_trd.isActive())
                break;
            */
        }
        sema.signal();

        r_trd.stop();
        l_trd.stop();

        Core::Thread::sleep(50);
        if( left.isActive() ) left.close();
        if( right.isActive() ) right.close();
    }

public:
    Network_Relay_Tcp_Bidirectional(Core::TCP_Client &_left, Core::TCP_Client &_right):
        left(_left),
        right(_right)
    {

    }

    virtual bool run()
    {
        relay();
        return true;
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// bob --> [relay server] --> alice
/// @todo need more test
class Relay_Server_Tcp : public Core::TCP_Server
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

        // if( left_ip.size() != 0 && left.getRemoteIP() != left_ip )
        //    return false;

        Core::TCP_Client right(right_ip, right_remote_port);

        Core::Logger::logItLn("[Server Relay] connecting...");
        if( !right.connect() )
            return false;

        Network_Relay_Tcp nrelay(left, right);
        nrelay.run();

        return true;
    }

public:
    Relay_Server_Tcp(
            const string &          client_ip,
            const unsigned int &    client_port,
            const string &          server_ip,
            const unsigned int &    server_port,
            const bool              multi_thread = true):
        TCP_Server(client_port, multi_thread)
    {
        Core::Logger::logVariable("Left", client_ip);
        Core::Logger::logVariable("Right", server_ip);
        Core::Logger::logVariable("LPort", client_port);
        Core::Logger::logVariable("RPort", server_port);

        right_ip = server_ip;
        left_ip = client_ip;
        right_remote_port = server_port;
    }

    ~Relay_Server_Tcp()throw()
    {
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Network_Relay_Udp
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class Network_Relay_Udp : public Core::Component
{
private:
    Core::TCP_Client &left;
    Core::UDP_Client &right;

protected:
    virtual void relay()
    {
        Core::Logger::logItLn("UDP Relaying...");

        Core::Logger::logItLn("[UDP Relay] starting listening thread...");

        unsigned char *data = _null_;
        size_t data_size;

        Core::Logger::logItLn("[UDP Relay] entering relay loop...");
        // while( left.isActive() )
        while ( left.isActive() && right.isActive() )
        {
            Core::Logger::logItLn("[UDP Relay] left recv");
            if( left.isActive() && !left.recv(data, data_size) ) break;
            Core::Logger::logItLn("[UDP Relay] right send");
            if( right.isActive() && !right.send(data, data_size) ) break;
            Core::Logger::logItLn("[UDP Relay] right recv");
            if( right.isActive() && !right.recv(data, data_size) ) break;
            Core::Logger::logItLn("[UDP Relay] left send");
            if( left.isActive() && !left.send(data, data_size) ) break;
        }

        Core::Thread::sleep(10);
        if( left.isActive() ) left.close();
        if( right.isActive() ) right.close();
    }

public:
    Network_Relay_Udp(Core::TCP_Client &_left, Core::UDP_Client &_right):
        left(_left),
        right(_right)
    {

    }

    virtual bool run()
    {
        relay();
        return true;
    }
};

}

}
#endif
