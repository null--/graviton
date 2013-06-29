/**
 * @file
 * 
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
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
 * @brief GraVitoN Config File
 *
*/

#ifndef GRAVITON_CONFIG_H
#define GRAVITON_CONFIG_H

//#define GVN_ACTIVATE_LUABRIDGE

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// My Lovely Define!
#define _null_ 0x00 //NULL

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// COMPILER OPTIONS
#ifdef INFO_COMPILER_GCC
    #pragma GCC diagnostic ignored "-Wdeprecated"
    #pragma GCC diagnostic ignored "-Wcpp"
    #pragma GCC diagnostic ignored "-Wignored-qualifiers"
    #pragma GCC diagnostic ignored "-Wnarrowing"
#endif

#ifdef INFO_COMPILER_MSVC
    #pragma warning( disable : 4996)
#endif

#ifdef INFO_OS_WINDOWS
	/// DO NOT NEED TO WRITE #include<windows.h> INSIDE YOUR SOURCE CODE, it may cause redefinition errors
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined(INFO_OS_LINUX)
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <errno.h>
    #include <unistd.h>
#elif defined(INFO_OS_OSX)
    #include <sys/mman.h>
    #include <errno.h>
#endif

#include <string>
#include <exception>
#include <stdexcept>

namespace GraVitoN
{
    /// Some typedefs
    typedef char                gchar;
    typedef unsigned char       guchar;
    typedef unsigned int        guint;
    typedef size_t              gsize;
    typedef unsigned long       gulong;

	namespace Config
	{
        /// Maximum packet size
		const guint     MAX_TCP_PACKET_SIZE = 4069;
        /// Maximum file size
		const gulong    MAX_FILE_SIZE = 32 * 1024 * 1024 * sizeof(guchar); // 64 MB
        /// Maximum xml value size
        const gsize     MAX_XML_VALUE_SIZE = 4069;
	}

    namespace Core
    {
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        /// Sleep for a number of miliseconds
        void sleep(gulong milisec)
        {
#ifdef INFO_OS_WINDOWS
            ::SleepEx(milisec, FALSE);// (float)milisec / 1000.f );
#else
            ::usleep( milisec * 1000);
#endif
        }
    }
}

#endif
