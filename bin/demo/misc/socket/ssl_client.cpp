#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/ssl_client.hpp>
#include <iostream>

/*
F:\null\project\GraVitoN\graviton\bin\demo\misc\socket>cl -D WIN32 -I ..\..\..\..\code ssl_client.cpp ws2_32.lib ..\..\..\..\lib\openssl\win-x86-msvc\libeay32.lib ..\..\..\..\lib\openssl\win-x86-msvc\ssleay32.lib kernel32.lib gdi32.lib advapi32.lib user32.lib /I ..\..\..\..\code\external\openssl\include
*/
using namespace std;

int main()
{
    // GraVitoN::Core::SSL_Client cln("127.0.0.1", 1111);

    GraVitoN::Core::SSL_Client cln("173.194.113.147", 443);

    if( cln.connect() )
    {
        cout << "Certificate:" << endl;
        string issuer, subject, cipher;
        cipher = cln.getCipher();
        cout << "\tCipher:" << cipher << endl;
        if( cln.getRemoteCertInfo(subject, issuer) )
        {
            cout << "\tSubject:" << subject << endl;
            cout << "\tIssuer:" << issuer << endl;
        }

        string data;
        cln.sendString("HEAD / HTTP/1.1\n\n");
        data = cln.recvString();
        cout << "Server: " << data << endl;
    }

    cout << "DONE" << endl;
    return 0;
}
