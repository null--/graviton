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
 * @brief GraVitoN Artitecture/OS/Compiler (AOC) Information header
 *
*/

/// @todo: MORE COMPILER, OS, and ARCH
#ifndef _GVN_AOCINFO_HEAD_
#define _GVN_AOCINFO_HEAD_

/// Detect compiler
#if defined(__GNUC__) || defined(__GNUG__)
    #define INFO_COMPILER_GCC /// GCC Compiler
#elif defined(_MSC_VER)
    #define INFO_COMPILER_MSVC /// MSVC Compiler
#else
    #define INFO_COMPILER_UNKNOWN /// Unknown Compiler
#endif

/// Detect CPU Artitecture
#if defined(_WIN64)
	#define INFO_CPU_X64 /// Windows 64 bit
#elif defined(WIN32)
	#define INFO_CPU_X32 /// Windows 32 bit
#elif defined(INFO_COMPILER_GCC)
    #if defined(__i386__) //__i386__ is defined for any x86 processor
        #define INFO_CPU_X86 /// x86 CPU
    #elif defined(__x86_64__)
        #define INFO_CPU_X64 /// x64 CPU
    #elif defined(__arm__)
        #define INFO_CPU_ARM /// ARM CPU
    #else
        #define INFO_CPU_UNKNOWN /// Unkown CPU
    #endif
#elif defined(INFO_COMPILER_MSVC)
    #if defined(_M_IX86)
        #define INFO_CPU_X32 /// x86 CPU
    #elif defined(_M_AMD64) || defined(_M_X64)
        #define INFO_CPU_X64 /// x64 CPU
    #else
        #define INFO_CPU_UNKNOWN /// Unknown CPU
    #endif
#else
    #define INFO_CPU_UNKNOWN
#endif

/// Detect OS
#if defined(__linux__)
    #define INFO_OS_LINUX /// Linux
#elif defined(_WIN64)
    #define INFO_OS_WINDOWS /// Windows 64 bit
#elif defined(WIN32)
    #define INFO_OS_WINDOWS /// Windows 32 bit
#elif defined(__APPLE__)
    #define INFO_OS_OSX /// OSX
#elif defined(sun) || defined(__sun)
    #define INFO_OS_SOLARIS /// Solaris
#elif defined(__ANDROID__)
    #define INFO_OS_ANDROID /// Android
#else
	#define INFO_OS_UNKNOWN /// Unkown OS
#endif

/**
 * Use CBLOCKS like this:
 * 
 * CBLOCK_WINDOWS
 * 		... code for windows ...
 * CBLOCK_END
 * CBLOCK_LINUX
 * 		... code for linux ...
 * CBLOCK_END
 */
#define CBLOCK_WINDOWS 	#ifdef INFO_OS_WINDOWS
#define CBLOCK_LINUX	#ifdef INFO_OS_LINUX
#define CBLOCK_OSX		#ifdef INFO_OS_OSX
#define CBLOCK_SOLARIS	#ifdef INFO_OS_SOLARIS
#define CBLOCK_ANDROID	#ifdef INFO_OS_ANDROID
#define CBLOCK_END		#endif

#endif // _GVN_AOCINFO_HEAD_
