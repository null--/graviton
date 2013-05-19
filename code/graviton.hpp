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
 * @brief GraVitoN Core File
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_HEAD_
#define _GVN_HEAD_

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// My Lovely Define!
#define _null_ 0x00 //NULL

/// Activate Logger
// #define GVN_ACTIVATE_LOGGER
#ifdef GVN_ACTIVATE_LOGGER
    #warning ("[INFO] LOGGER IS AVTIVATED (GVN_ACTIVATE_LOGGER)")
#endif
/// Lua Verbose Mode
// #define GVN_VERBOSE_LUA
#ifdef GVN_VERBOSE_LUA
    #warning ("[INFO] LUA VERBOSE MODE IS ACTIVE (GVN_VERBOSE_LUA)")
#endif
/// Path to log file
// #define GVN_LOG_FILE "graviton.log"
#ifdef GVN_LOG_FILE
    #warning ("[INFO] LOG FILE IS SET (GVN_LOG_FILE)")
#endif
/// DEBUG MODE
// #define GVN_DEBUG
#ifdef GVN_DEBUG
    #warning ("[WARNING] DEBUG MODE IS ENABLED")
#endif

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Detect Artitecture/OS/Compiler
#include "core/aocinfo.hpp"
#include "core/component.hpp"

// SOME COMPILER OPTIONS
#ifdef INFO_COMPILER_GCC
    #pragma GCC diagnostic ignored "-Wdeprecated"
    #pragma GCC diagnostic ignored "-Wcpp"
    #pragma GCC diagnostic ignored "-Wignored-qualifiers"
    #pragma GCC diagnostic ignored "-Wnarrowing"
#endif

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo: ai, payload, infect, malkit, utils
// #include <ai/ai.hpp>
// #include <payload/payload.hpp>
// #include <infect/infect.hpp>
// #include <malkit/malkit.hpp>
// #include <utils/utils.hpp>


namespace GraVitoN
{
	const unsigned int MAX_TCP_PACKET_SIZE = 60000;
}


#endif // _GVN_HEAD_
