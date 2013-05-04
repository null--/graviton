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
 * @author Ivan Gagis <igagis@gmail.com>
 * @author Jose Luis Hidalgo <joseluis.hidalgo@gmail.com> - Mac OS X port
 */

#pragma once

#include "../config.hpp"
#include "../debug.hpp"


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

//TODO:
//#	include <process.h>

#elif M_OS == M_OS_SYMBIAN
#	include <string.h>
#	include <e32std.h>
#	include <hal.h>

#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX || M_OS == M_OS_SOLARIS
#	include <pthread.h>

#else
#	error "Unsupported OS"
#endif



namespace ting{
namespace mt{



/**
 * @brief Mutex object class
 * Mutex stands for "Mutual execution".
 */
class Mutex{

#if M_OS == M_OS_WINDOWS
	CRITICAL_SECTION m;
#elif M_OS == M_OS_SYMBIAN
	RCriticalSection m;
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
	friend class CondVar;
	pthread_mutex_t m;
#else
#	error "unknown OS"
#endif

private:
	//forbid copying
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);

public:
	/**
	 * @brief Creates initially unlocked mutex.
	 */
	Mutex();

	~Mutex()throw();


	/**
	 * @brief Acquire mutex lock.
	 * If one thread acquired the mutex lock then all other threads
	 * attempting to acquire the lock on the same mutex will wait until the
	 * mutex lock will be released with Mutex::Unlock().
	 */
	inline void Lock()throw(){
//		TRACE(<< "Mutex::Lock(): invoked " << this << std::endl)
#if M_OS == M_OS_WINDOWS
		EnterCriticalSection(&this->m);
#elif M_OS == M_OS_SYMBIAN
		this->m.Wait();
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
		pthread_mutex_lock(&this->m);
#else
#	error "unknown OS"
#endif
	}



	/**
	 * @brief Release mutex lock.
	 */
	inline void Unlock()throw(){
//		TRACE(<< "Mutex::Unlock(): invoked " << this << std::endl)
#if M_OS == M_OS_WINDOWS
		LeaveCriticalSection(&this->m);
#elif M_OS == M_OS_SYMBIAN
		this->m.Signal();
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
		pthread_mutex_unlock(&this->m);
#else
#	error "unknown OS"
#endif
	}



	/**
	 * @brief Helper class which automatically Locks the given mutex.
	 * This helper class automatically locks the given mutex in the constructor and
	 * unlocks the mutex in destructor. This class is useful if the code between
	 * mutex lock/unlock may return or throw an exception,
	 * then the mutex will be automatically unlocked in such case.
	 */
	class Guard{
		Mutex &mutex;

		//forbid copying
		Guard(const Guard& );
		Guard& operator=(const Guard& );
	public:
		Guard(Mutex &m)throw() :
				mutex(m)
		{
			this->mutex.Lock();
		}
		~Guard()throw(){
			this->mutex.Unlock();
		}
	};//~class Guard
};//~class Mutex



}//~namespace
}//~namespace
