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
/// bob ---> [relay server] ---> alice
class Network_Relay_TCP_Forward : public Core::Component
{
private:
    Core::TCP_Client &left;
    Core::TCP_Client &right;

protected:
    virtual void relay()
    {
        Core::Logger::logItLn("Relaying (FW)...");

        right.connect();

        unsigned char *data = _null_;
        size_t data_size;

        Core::Logger::logItLn("[Relay (FW)] entering relay loop...");
        // while( left.isActive() )
        while ( 1 )
        {
            // Core::Logger::logItLn("[Relay] left recv");
            if( !left.recv(data, data_size) )
                break;
            // Core::Logger::logItLn("[Relay] right send");
            right.send(data, data_size);
            // Core::Logger::logItLn("[Relay] right recv");
            right.recv(data, data_size);
            // Core::Logger::logItLn("[Relay] left send");
            left.send(data, data_size);
        }
        Core::Logger::logItLn("[Relay (FW)] loop done");

        Core::Thread::sleep(10);
        if( left.isActive() ) left.close();
        if( right.isActive() ) right.close();
    }

public:
    Network_Relay_TCP_Forward(Core::TCP_Client &_left, Core::TCP_Client &_right):
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
/// bob <---> [relay server] <---> alice
class Network_Relay_TCP : public Core::Component
{
private:
    Core::TCP_Client &left;
    Core::TCP_Client &right;

    class RELAY_THREAD: public Core::Thread
    {
    protected:
        Core::TCP_Client &sock1;
        Core::TCP_Client &sock2;
        // Core::Semaphore &sema;

        virtual bool myMainLoop()
        {
            // Core::Logger::logItLn("[Relay Thread] entering relay loop...");
            while( sock1.isActive()  )
            {
                unsigned char *data = _null_;
                size_t data_size;

                // sema.wait();
                // Core::Logger::logItLn("[Relay Thread] recv");
                if( sock1.recv(data, data_size) )
                {
                    Core::Logger::logVariable("[Relay Thread] recv from IP: ", sock1.getRemoteIP());
                    Core::Logger::logVariable("[Relay Thread] recv from PORT: ", sock1.getRemotePort());
                    // Core::Logger::logItLn("[Relay Thread] send");

                    // sema.wait();
                    bool res = sock2.send(data, data_size);
                    // sema.signal();

                    Core::Logger::logVariable("[Relay Thread] send IP: ", sock2.getRemoteIP());
                    Core::Logger::logVariable("[Relay Thread] send PORT: ", sock2.getRemotePort());

                    if( !res )
                    {
                        Core::Logger::logVariable("[Relay Thread] Connection Died! ", sock2.getRemoteIP());
                        // if( !sock2.connect() )
                            break;
                    }
                }
                else
                {
                    // if( !sock1.connect() )
                    Core::Logger::logVariable("[Relay Thread] Connection Died! ", sock2.getRemoteIP());
                        break;
                }
                // sema.signal();
            }
            Core::Logger::logItLn("[Relay Thread] Done");
            return false;
        }

    public:
        RELAY_THREAD(
                Core::TCP_Client &p_sock1,
                Core::TCP_Client &p_sock2
                //Core::Semaphore &p_sema
                ):
            sock1(p_sock1),
            sock2(p_sock2)
            //sema(p_sema)
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

        // Core::Logger::logItLn("[Relay] starting listening thread...");
        // Core::Semaphore sema;

        RELAY_THREAD l_trd(left, right); //, sema);
        RELAY_THREAD r_trd(right, left); //, sema);

        if( !right.connect() )
            return;

        l_trd.run();
        r_trd.run();

        // unsigned char *data = _null_;
        // size_t data_size;

        Core::Logger::logItLn("[Relay] entering relay loop...");
        // sema.signal();
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
        Core::Logger::logItLn("[Relay] loop done");
        // sema.signal();

        r_trd.stop();
        l_trd.stop();

        Core::Thread::sleep(50);
        if( left.isActive() ) left.close();
        if( right.isActive() ) right.close();
    }

public:
    Network_Relay_TCP(Core::TCP_Client &_left, Core::TCP_Client &_right):
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
/// 1) bob ---> [relay server] --- alice
/// 2) bob <---> [relay server] <---> alice
/// @todo needs more test
class Network_Relay_TCP_Server : public Core::TCP_Server
{
protected:
    // string left_ip;
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

        Network_Relay_TCP nrelay(left, right);
        nrelay.run();

        return true;
    }

public:
    Network_Relay_TCP_Server(
            const unsigned int &    listen_port,
            const string &          remote_ip,
            const unsigned int &    remote_port,
            const bool              multi_thread = true):
        TCP_Server(listen_port, multi_thread)
    {
        // Core::Logger::logVariable("Left", client_ip);
        // Core::Logger::logVariable("Right", server_ip);
        // Core::Logger::logVariable("LPort", client_port);
        // Core::Logger::logVariable("RPort", server_port);

        right_ip = remote_ip;
        // left_ip = client_ip;
        right_remote_port = remote_port;
    }

    ~Network_Relay_TCP_Server()throw()
    {
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// bob <-- udp --> [relay server] <-- udp --> alice
/// @todo more tests
class Network_Relay_UDP : public Core::Component
{
private:
    unsigned int    local_port;
    string          left_ip;
    unsigned int    left_port;
    string          right_ip;
    unsigned int    right_port;

protected:
    virtual void relay()
    {
        Core::Logger::logItLn("Relaying (TCP/UDP)...");

        Core::UDP_Socket server;

        if( !server.open(local_port) )
            return;

        unsigned char *data = _null_;
        size_t data_size;
        unsigned int sender_port, dest_port;
        string sender_ip, dest_ip;
        while( server.isActive() )
        {
            if( !server.recv(data, data_size, sender_ip, sender_port) )
            {
                break;
            }

            Core::Logger::logVariable("[Relay UDP] Recv IP: ", sender_ip);
            Core::Logger::logVariable("[Relay UDP] Recv Port: ", sender_port);

            if( sender_ip == right_ip && sender_port == right_port)
            {
                dest_ip = left_ip;
                dest_port = left_port;
            }
            else
            {
                dest_ip = right_ip;
                dest_port = right_port;
            }

            Core::Logger::logVariable("[Relay UDP] Send IP: ", dest_ip);
            Core::Logger::logVariable("[Relay UDP] Send Port: ", dest_port);

            if( !server.send(data, data_size, dest_ip, dest_port) )
            {
                break;
            }
        }

        server.close();
        Core::Thread::sleep(50);
    }

public:
    Network_Relay_UDP(const unsigned int &_local_port,
                      const string &_left_ip,
                      const unsigned int &_left_port,
                      const string &_right_ip,
                      const unsigned int &_right_port)
    {
        local_port = _local_port;
        left_ip = _left_ip;
        left_port = _left_port;
        right_ip = _right_ip;
        right_port = _right_port;
    }

    virtual bool run()
    {
        // Core::Logger::logVariable("[Relay UDP] Left IP: ", left_ip);
        // Core::Logger::logVariable("[Relay UDP] Right IP: ", right_ip);

        relay();
        return true;
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// bob <-- tcp --> [relay server] <-- udp --> alice
/// @todo more tests
class Network_Relay_TCP_To_UDP : public Core::Component
{
private:
    Core::TCP_Client &left;
    string right_ip;
    unsigned int right_port;
    unsigned int udp_port;

    class RELAY_THREAD: public Core::Thread
    {
    protected:
        Core::TCP_Client &sock1;
        Core::UDP_Socket &sock2;
        string right_ip;
        unsigned int right_port;

        virtual bool myMainLoop()
        {
            while( sock1.isActive()  )
            {
                unsigned char *data = _null_;
                size_t data_size;

                if( sock1.recv(data, data_size) )
                {
                    Core::Logger::logVariable("[Relay [TUDP] Thread] recv from: ", sock1.getRemoteIP());

                    bool res = sock2.send(data, data_size, right_ip, right_port);

                    Core::Logger::logVariable("[Relay [TUDP] Thread] send to IP: ", right_ip);
                    Core::Logger::logVariable("[Relay [TUDP] Thread] send to PORT: ", right_port);

                    if( !res )
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            return false;
        }

    public:
        RELAY_THREAD(
                Core::TCP_Client &p_sock1,
                Core::UDP_Socket &p_sock2,
                const string &_right_ip,
                const unsigned int &_right_port
                ):
            sock1(p_sock1),
            sock2(p_sock2)
        {
            right_ip = _right_ip;
            right_port = _right_port;
        }

        ~RELAY_THREAD()throw()
        {

        }
    };

public:
    /// udp_port == 0 --> random port
    Network_Relay_TCP_To_UDP(Core::TCP_Client &_left,
                             const string &_right_ip,
                             const unsigned int &_right_port,
                             const unsigned int &_udp_port = 0):
        left(_left)
    {
        right_ip = _right_ip;
        right_port = _right_port;
        udp_port = _udp_port;
    }

    virtual bool run()
    {
        Core::UDP_Socket right;
        if( !right.open(udp_port) )
            return false;

        // cout << "F1" << endl;
        RELAY_THREAD trd(left, right, right_ip, right_port);
        // cout << "F2" << endl;
        trd.run();

        unsigned char *data = _null_;
        size_t data_size;

        // cout << "F3" << endl;
        while( trd.isActive() )
        {
            // cout << "F4" << endl;
            if( right.recv(data,data_size) )
            {
                // cout << "F5" << endl;
                if( !left.send(data, data_size) )
                {
                    break;
                }
            }
        }

        left.close();
        right.close();

        return true;
    }
};

}
}
#endif
