/**
 * @file
 *
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
 * @brief GraVitoN::Thread
 *
 * @todo IT'S NOT COMPELTED YET
*/

#ifndef _GVN_THREAD_HEAD_
#define _GVN_THREAD_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <external/ting/net/Lib.hpp>
#include <external/ting/mt/Thread.hpp>

namespace GraVitoN
{
	
namespace Core
{
	
namespace Thread
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Cross Platform OS version sleep function
void Sleep(unsigned msec)
{
	ting::mt::Thread::Sleep( msec );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
typedef struct ThreadObj
{
	class _T_THREAD : public ting::mt::Thread 
	{
		void*(void*) prt_mainloop_function;
		
	} i_thread;
	
	bool i_stop_flag;
	void*(void*) prt_mainloop_function;
	
	ThreadObj()
	{
		i_stop_flag = true;
		ptr_mainloop_func = _null_;
	}
} & R_ThreadObj;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Thread::~Thread() throw()
{
	stop();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Thread::initialize(const string &_options)
{
	options = _options;

	if( !flag_stop )
	{
		stop();
	}

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Thread::run()
{
	ting::mt::Thread::Start();
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Thread::waitForEndOfMainLoop()
{
	ting::mt::Thread::Join();
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
void Thread::Run()
{
	flag_stop = false;

	this->myMainLoop();

	flag_stop = true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Thread::myMainLoop()
{
	if( !flag_stop )
		Logger::logItLn("[Thread] I'm Running!");
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Thread::isActive()
{
	return !flag_stop;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Thread::stop()
{
	flag_stop = true;

	waitForEndOfMainLoop();

	return true;
}

} // end of Thread
} // end of Core
} // end of GraVitoN

#endif // _GVN_THREAD_HEAD_
