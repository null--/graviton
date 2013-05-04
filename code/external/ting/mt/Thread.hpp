/* The MIT License:

Copyright (c) 2008-2012 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://ting.googlecode.com



/**
 * @file Thread.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @author Jose Luis Hidalgo <joseluis.hidalgo@gmail.com> - Mac OS X port
 * @brief Multithreading library.
 */

#pragma once

#include "../config.hpp"
#include "../debug.hpp"
#include "../types.hpp"
#include "../Exc.hpp"

#include "Mutex.hpp"



#if M_OS == M_OS_WINDOWS

//if _WINSOCKAPI_ macro is not defined then it means that the winsock header file
//has not been included. Here we temporarily define the macro in order to prevent
//inclusion of winsock.h from within the windows.h. Because it may later conflict with
//winsock2.h if it is included later.
#	ifndef _WINSOCKAPI_
#		define _WINSOCKAPI_
#		include <windows.h>
#		undef _WINSOCKAPI_
#	else
#		include <windows.h>
#	endif

#elif M_OS == M_OS_SYMBIAN
#	include <string.h>
#	include <e32std.h>
#	include <hal.h>

#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX || M_OS == M_OS_SOLARIS
#	include <pthread.h>

#	if M_OS == M_OS_SOLARIS
#		include <sched.h> // for sched_yield();
#	endif

#else
#	error "Unsupported OS"
#endif



namespace ting{
namespace mt{



/**
 * @brief a base class for threads.
 * This class should be used as a base class for thread objects, one should override the
 * Thread::Run() method.
 */
class Thread{

//Tread Run function
#if M_OS == M_OS_WINDOWS
	static unsigned int __stdcall RunThread(void *data);
#elif M_OS == M_OS_SYMBIAN
	static TInt RunThread(TAny *data);
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
	static void* RunThread(void *data);
#else
#	error "Unsupported OS"
#endif


	ting::mt::Mutex mutex1;


	enum E_State{
		NEW,
		RUNNING,
		STOPPED,
		JOINED
	};
	
	ting::Inited<volatile E_State, NEW> state;

	//system dependent handle
#if M_OS == M_OS_WINDOWS
	HANDLE th;
#elif M_OS == M_OS_SYMBIAN
	RThread th;
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
	pthread_t th;
#else
#	error "Unsupported OS"
#endif

	//forbid copying
	Thread(const Thread& );
	Thread& operator=(const Thread& );

public:
	
	/**
	 * @brief Basic exception type thrown by Thread class.
	 * @param msg - human friendly exception description.
	 */
	class Exc : public ting::Exc{
	public:
		Exc(const std::string& msg) :
				ting::Exc(msg)
		{}
	};
	
	class HasAlreadyBeenStartedExc : public Exc{
	public:
		HasAlreadyBeenStartedExc() :
				Exc("The thread has already been started.")
		{}
	};
	
	Thread();
	
	
	virtual ~Thread()throw();



	/**
	 * @brief This should be overridden, this is what to be run in new thread.
	 * Pure virtual method, it is called in new thread when thread runs.
	 */
	virtual void Run() = 0;



	/**
	 * @brief Start thread execution.
	 * Starts execution of the thread. Thread's Thread::Run() method will
	 * be run as separate thread of execution.
	 * @param stackSize - size of the stack in bytes which should be allocated for this thread.
	 *                    If stackSize is 0 then system default stack size is used
	 *                    (stack size depends on underlying OS).
	 */
	void Start(size_t stackSize = 0);



	/**
	 * @brief Wait for thread to finish its execution.
	 * This function waits for the thread finishes its execution,
	 * i.e. until the thread returns from its Thread::Run() method.
	 * Note: it is safe to call Join() on not started threads,
	 *       in that case it will return immediately.
	 */
	void Join()throw();



	/**
	 * @brief Suspend the thread for a given number of milliseconds.
	 * Suspends the thread which called this function for a given number of milliseconds.
	 * This function guarantees that the calling thread will be suspended for
	 * AT LEAST 'msec' milliseconds.
	 * @param msec - number of milliseconds the thread should be suspended.
	 */
	static void Sleep(unsigned msec = 0)throw(){
#if M_OS == M_OS_WINDOWS
		SleepEx(DWORD(msec), FALSE);// Sleep() crashes on MinGW (I do not know why), this is why SleepEx() is used here.
#elif M_OS == M_OS_SYMBIAN
		User::After(msec * 1000);
#elif M_OS == M_OS_SOLARIS || M_OS == M_OS_MACOSX || M_OS == M_OS_LINUX
		if(msec == 0){
#	if M_OS == M_OS_SOLARIS || M_OS == M_OS_MACOSX || defined(__ANDROID__)
			sched_yield();
#	elif M_OS == M_OS_LINUX
			pthread_yield();
#	else
#		error "Unsupported OS"
#	endif
		}else{
			usleep(msec * 1000);
		}
#else
#	error "Unsupported OS"
#endif
	}



	/**
	 * @brief Thread ID type.
	 * Thread ID type is used to identify a thread.
	 * The type supports operator==() and operator!=() operators.
	 */
	typedef unsigned long int T_ThreadID;



	/**
	 * @brief get current thread ID.
	 * Returns unique identifier of the currently executing thread. This ID can further be used
	 * to make assertions to make sure that some code is executed in a specific thread. E.g.
	 * assert that methods of some object are executed in the same thread where this object was
	 * created.
	 * @return unique thread identifier.
	 */
	static inline T_ThreadID GetCurrentThreadID()throw(){
#if M_OS == M_OS_WINDOWS
		return T_ThreadID(GetCurrentThreadId());
#elif M_OS == M_OS_MACOSX || M_OS == M_OS_LINUX
		pthread_t t = pthread_self();
		STATIC_ASSERT(sizeof(pthread_t) <= sizeof(T_ThreadID))
		return T_ThreadID(t);
#else
#	error "Unsupported OS"
#endif
	}
};



}//~namespace
}//~namespace
