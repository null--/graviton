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
#ifndef GRAVITON_H
#define GRAVITON_H

/// Detect Artitecture/OS/Compiler
#include <core/aocinfo.hpp>
#include "config.hpp"

/// ACTIVATE LUA BRIDGE
// #define GVN_ACTIVATE_LUABRIDGE
#ifndef GVN_ACTIVATE_LUABRIDGE
    #if defined(INFO_COMPILER_GCC)
        #warning ("[WARNING] LUABRIDGE IS NOT ACTIVATED")
    #else
    #endif
#endif
/// Activate Logger
// #define GVN_ACTIVATE_LOGGER
#ifdef GVN_ACTIVATE_LOGGER
    #if defined(INFO_COMPILER_GCC)
        #warning ("[INFO] LOGGER IS ACTIVATED (GVN_ACTIVATE_LOGGER)")
    #endif
#endif
/// Lua Verbose Mode
// #define GVN_VERBOSE_LUA
#ifdef GVN_VERBOSE_LUA
    #if defined(INFO_COMPILER_GCC)
        #warning ("[INFO] LUA VERBOSE MODE IS ACTIVE (GVN_VERBOSE_LUA)")
    #endif
#endif
/// Path to log file
// #define GVN_LOG_FILE "graviton.log"
#ifdef GVN_LOG_FILE
    #if defined(INFO_COMPILER_GCC)
        #warning ("[INFO] LOG FILE IS SET (GVN_LOG_FILE)")
    #endif
#endif
/// DEBUG MODE
// #define GVN_DEBUG
#ifdef GVN_DEBUG
    #if defined(INFO_COMPILER_GCC)
        #warning ("[WARNING] DEBUG MODE IS ENABLED")
    #endif
#endif

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifdef GVN_ACTIVATE_LUABRIDGE
    #include <core/luabridge.hpp>
    #include <core/luaviton.hpp>
#endif

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <core/component.hpp>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <core/logger.hpp>
// #include <ai/ai.hpp>
// #include <payload/payload.hpp>
// #include <infect/infect.hpp>
// #include <malkit/malkit.hpp>

#ifdef INFO_COMPILER_MSVC
    #define snprintf _snprintf
#endif
/// One namespace to rule them all
namespace GraVitoN
{

}


#endif // _GVN_HEAD_
