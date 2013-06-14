#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <core/ssl_client.hpp>

/*
cl ..\..\..\libs\openssl\win-x86-msvc\libeay32.lib ..\..\..\libs\openssl\win-x86-msvc\ssleay32.lib /I ..\..\..\code /I ..\..\..\code\external\openssl\include\ /D WIN32 kernel32.lib user32.lib gdi32.lib wsock32.lib advapi32.lib /GS /EHsc ssl_client.cpp
*/
using namespace std;

int main()
{
    // GraVitoN::Core::SSL_Client cln("127.0.0.1", 1111);

    GraVitoN::Core::SSL_Client cln(GraVitoN::Utils::Netkit::dnsLookup4("www.google.com"), 443);

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
