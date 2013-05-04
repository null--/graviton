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

#ifndef _GVN_AOCINFO_HEAD_
#define _GVN_AOCINFO_HEAD_

/// Detect compiler
#if defined(__GNUC__) || defined(__GNUG__)
    #define GVN_COMPILER_GCC /// GCC Compiler
#elif defined(_MSC_VER)
    #define GVN_COMPILER_MSVC /// MSVC Compiler
#else
    #define GVN_COMPILER_UNKNOWN /// Unknown Compiler
#endif

/// Detect CPU Artitecture
#if defined(GVN_COMPILER_GCC)
    #if defined(__i386__) //__i386__ is defined for any x86 processor
        #define GVN_CPU_X86 /// X86 CPU
    #elif defined(__x86_64__)
        #define GVN_CPU_X64 /// x64 CPU
    #elif defined(__arm__)
        #define GVN_CPU_ARM /// ARM CPU
    #else
        #define GVN_CPU_UNKNOWN /// Unkown CPU
    #endif
#elif defined(GVN_COMPILER_MSVC)
    #if defined(_M_IX86)
        #define GVN_CPU_X86 /// x86 CPU
    #elif defined(_M_AMD64) || defined(_M_X64)
        #define GVN_CPU_X86_64 /// x64 CPU
    #else
        #define GVN_CPU_UNKNOWN /// Unknown CPU
    #endif
#else
    #define GVN_CPU_UNKNOWN
#endif

/// Detect OS
#if defined(__linux__)
    #define GVN_OS_LINUX /// Linux
#elif defined(_WIN64)
    #define GVN_OS_WINDOWS /// Windows 64 bit
#elif defined(WIN32)
    #define GVN_OS_WINDOWS /// Windows 32 bit
#elif defined(__APPLE__)
    #define GVN_OS_MACOSX /// Mac OSX
#elif defined(sun) || defined(__sun)
    #define GVN_OS_SOLARIS /// Solaris
#elif defined(__ANDROID__)
    #define GVN_OS_ANDROID /// Android
#else
        #define GVN_OS_UNKNOWN /// Unkown OS
#endif

#endif // _GVN_AOCINFO_HEAD_
