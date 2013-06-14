#ifndef _GVN_CONFIG_HEAD_
#define _GVN_CONFIG_HEAD_

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
	/// YOU DO NOT NEED TO WRITE #include<windows.h> INSIDE YOUR SOURCE CODE
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace GraVitoN
{
	namespace Config
	{
		const unsigned int MAX_TCP_PACKET_SIZE = 4069;
		const unsigned long MAX_FILE_SIZE = 32 * 1024 * 1024 * sizeof(unsigned char); // 64 MB
	}
}

#endif
