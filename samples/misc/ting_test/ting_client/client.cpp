
/// Tested with ting 1.0.0
/// based on: http://code.google.com/p/ting/wiki/SocketsUsage
///
/// Run netcat as server: netcat -lvp 7357
///
#include <iostream>
#include "net/Lib.hpp"
#include "net/TCPSocket.hpp"
#include "net/TCPServerSocket.hpp"

int main()
{
    /// In case of errors the ting::Socket::Exc exception is thrown
    /// So, enclose the network code into try/catch.
    try
    {
        /// Initialize the library by creating a singletone object.
        /// It will deinitialize the library when the object
        /// will be destroyed due to going out of scope.
        ting::net::Lib socketsLib;

        /// create IP address for connecting
        ting::net::IPAddress ip("127.0.0.1", 7357);//we will connect to localhost 80th port

        ting::net::TCPSocket sock;

        /// connect to remote listening socket.
        /// It is an asynchronous operation, so we will use WaitSet
        /// to wait for its completion.
        sock.Open(ip);

        ting::WaitSet waitSet(1);
        waitSet.Add(&sock, ting::Waitable::WRITE);
        waitSet.Wait(); //< Wait for connection to complete.

		/// receive data (expecting 256 bytes (at max))
        ting::StaticBuffer<ting::u8, 256> data;
        unsigned bytesReceived = 0;

        while( bytesReceived == 0 )
            bytesReceived += sock.Recv(data, bytesReceived);

        std::cout<<bytesReceived<<" Bytes received: ";
        for(int i=0; i<bytesReceived; ++i)
            std::cout<<data[i];
        std::cout<<std::endl;
    }
    catch(ting::net::Exc &e)
    {
        std::cout << "Network error: " << e.What() << std::endl;
    }

    return 0;
}
