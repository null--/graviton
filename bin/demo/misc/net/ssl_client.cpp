/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief test ssl client
 *
 */

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
    GraVitoN::Core::SSL_Client cln( GraVitoN::Core::Socket::dnsLookup4("www.google.com"), 443);
    // GraVitoN::Core::SSL_Client cln("192.168.56.1", 1111);

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
        // cln.sendString("Do you hear me?");
        data = cln.recvString();
        cout << "Server: " << data << endl;
    }

    cout << "DONE" << endl;
    return 0;
}
