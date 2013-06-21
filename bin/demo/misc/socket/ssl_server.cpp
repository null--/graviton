#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <core/ssl_server.hpp>

using namespace std;
/*
cl ..\..\..\libs\openssl\win-x86-msvc\libeay32.lib ..\..\..\libs\ting\win-x86-msvc\libting.lib ..\..\..\libs\openssl\win-x86-msvc\ssleay32.lib /I ..\..\..\code /I ..\..\..\code\external\ting /I ..\..\..\code\external\openssl\include\ /D WIN32 kernel32.lib user32.lib gdi32.lib wsock32.lib advapi32.lib /GS /EHsc ssl_client.cpp
*/
/// Generate PEM File:
/// openssl genrsa -out domain.tld.key 1024
/// openssl req -new -key domain.tld.key -out domain.tld.csr
/// openssl x509 -req -days 365 -in domain.tld.csr -signkey domain.tld.key -out domain.tld.crt
/// cat domain.tld.key domain.tld.crt > key.pem

class My_SSL_Server : public GraVitoN::Core::SSL_Server
{
public:
    My_SSL_Server() : GraVitoN::Core::SSL_Server(1111, "key.pem", "key.pem")
    {

    }

    virtual bool response(GraVitoN::Core::SSL_Client &client_sock)
    {
        cout << "Certificate:" << endl;
        string issuer, subject, cipher;
        cipher = client_sock.getCipher();
        cout << "\tCipher:" << cipher << endl;
        if( client_sock.getRemoteCertInfo(subject, issuer) )
        {
            cout << "\tSubject:" << subject << endl;
            cout << "\tIssuer:" << issuer << endl;
        }

        string data;
        data = client_sock.recvString();
        cout << "Client: '" << data << "'" << endl;
        // GraVitoN::Core::Thread::sleep(1000);
        client_sock.sendString("I Hear You, man!");
        return true;
    }
};

int main()
{
    My_SSL_Server serv;
    serv.run();

    return true;
}
