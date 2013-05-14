
/// Tested with ting 1.0.0
/// based on: http://code.google.com/p/ting/wiki/SocketsUsage
///
/// Run netcat as client: netcat 127.0.0.1 7357
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

        /// create TCP server socket for listening
        ting::net::TCPServerSocket listenSock;

        /// open listening socket and start listening
        listenSock.Open(7357);//start listening on 80th port

        /// Accept some connection
        ting::net::TCPSocket sock;

        /// Accept first 3 incomming connections
        int cons = 0;
        while(listenSock.IsValid())
        {
            /// Check for waiting connection
            sock = listenSock.Accept();

            /// Validate socket
            if( !sock.IsValid() ) continue;

            /// Update number of connections
            ++cons;

            /// Send some data
            unsigned char hello[8] = "hello!\n";
            ting::Buffer<unsigned char> data(hello, 8);
            sock.Send(data);
            sock.Close();

            /// Close socket if it's 3rd connection
            if( cons == 3 )
                listenSock.Close();
        }

    }
    catch(ting::net::Exc &e)
    {
        std::cout << "Network error: " << e.What() << std::endl;
    }

    return 0;
}
