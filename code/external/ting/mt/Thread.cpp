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

#include "Thread.hpp"


#if M_OS == M_OS_WINDOWS
#	include <process.h>
#endif



using namespace ting::mt;



namespace{



Mutex threadMutex2;



}//~namespace



//Tread Run function
//static
#if M_OS == M_OS_WINDOWS
unsigned int __stdcall Thread::RunThread(void *data)
#elif M_OS == M_OS_SYMBIAN
TInt Thread::RunThread(TAny *data)
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
void* Thread::RunThread(void *data)
#else
#	error "Unsupported OS"
#endif
{
	Thread *thr = reinterpret_cast<Thread*>(data);
	try{
		thr->Run();
	}catch(ting::Exc& e){
		ASSERT_INFO(false, "uncaught ting::Exc exception in Thread::Run(): " << e.What())
	}catch(std::exception& e){
		ASSERT_INFO(false, "uncaught std::exception exception in Thread::Run(): " << e.what())
	}catch(...){
		ASSERT_INFO(false, "uncaught unknown exception in Thread::Run()")
	}

	{
		//protect by mutex to avoid changing the
		//this->state variable before Start() has finished.
		Mutex::Guard mutexGuard(threadMutex2);

		thr->state = STOPPED;
	}

#if M_OS == M_OS_WINDOWS
	//Do nothing, _endthreadex() will be called   automatically
	//upon returning from the thread routine.
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
	pthread_exit(0);
#else
#	error "Unsupported OS"
#endif
	return 0;
}



Thread::Thread(){
#if M_OS == M_OS_WINDOWS
	this->th = NULL;
#elif M_OS == M_OS_SYMBIAN
	//do nothing
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
	//do nothing
#else
#	error "Unsuported OS"
#endif
}



Thread::~Thread()throw(){
	ASSERT_INFO(
			this->state == JOINED || this->state == NEW,
			"~Thread() destructor is called while the thread was not joined before. "
			<< "Make sure the thread is joined by calling Thread::Join() "
			<< "before destroying the thread object."
		)

	//NOTE: it is incorrect to put this->Join() to this destructor, because
	//thread shall already be stopped at the moment when this destructor
	//is called. If it is not, then the thread will be still running
	//when part of the thread object is already destroyed, since thread object is
	//usually a derived object from Thread class and the destructor of this derived
	//object will be called before ~Thread() destructor.
}



void Thread::Start(size_t stackSize){
	//Protect by mutex to avoid several Start() methods to be called
	//by concurrent threads simultaneously and to protect call to Join() before Start()
	//has returned.
	Mutex::Guard mutexGuard1(this->mutex1);
	
	//Protect by mutex to avoid incorrect state changing in case when thread
	//exits before the Start() method returned.
	Mutex::Guard mutexGuard2(threadMutex2);

	if(this->state != NEW){
		throw HasAlreadyBeenStartedExc();
	}

#if M_OS == M_OS_WINDOWS
	this->th = reinterpret_cast<HANDLE>(
			_beginthreadex(
					NULL,
					stackSize > size_t(unsigned(-1)) ? unsigned(-1) : unsigned(stackSize),
					&RunThread,
					reinterpret_cast<void*>(this),
					0,
					NULL
				)
		);
	if(this->th == NULL){
		throw Exc("Thread::Start(): _beginthreadex failed");
	}
#elif M_OS == M_OS_SYMBIAN
	if(this->th.Create(_L("ting thread"), &RunThread,
				stackSize == 0 ? KDefaultStackSize : stackSize,
				NULL, reinterpret_cast<TAny*>(this)) != KErrNone
			)
	{
		throw Exc("Thread::Start(): starting thread failed");
	}
	this->th.Resume();//start the thread execution
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
	{
		pthread_attr_t attr;

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_attr_setstacksize(&attr, stackSize);

		int res = pthread_create(&this->th, &attr, &RunThread, this);
		if(res != 0){
			pthread_attr_destroy(&attr);
			TRACE_AND_LOG(<< "Thread::Start(): pthread_create() failed, error code = " << res
					<< " meaning: " << strerror(res) << std::endl)
			std::stringstream ss;
			ss << "Thread::Start(): starting thread failed,"
					<< " error code = " << res << ": " << strerror(res);
			throw Exc(ss.str());
		}
		pthread_attr_destroy(&attr);
	}
#else
#	error "Unsupported OS"
#endif
	this->state = RUNNING;
}



void Thread::Join() throw(){
//	TRACE(<< "Thread::Join(): enter" << std::endl)

	//protect by mutex to avoid several Join() methods to be called by concurrent threads simultaneously.
	//NOTE: excerpt from pthread docs: "If multiple threads simultaneously try to join with the same thread, the results are undefined."
	Mutex::Guard mutexGuard(this->mutex1);

	if(this->state == NEW){
		//thread was not started, do nothing
		return;
	}

	if(this->state == JOINED){
		return;
	}

	ASSERT(this->state == RUNNING || this->state == STOPPED)
	
	ASSERT_INFO(T_ThreadID(this->th) != ting::Thread::GetCurrentThreadID(), "tried to call Join() on the current thread")

#if M_OS == M_OS_WINDOWS
	WaitForSingleObject(this->th, INFINITE);
	CloseHandle(this->th);
	this->th = NULL;
#elif M_OS == M_OS_SYMBIAN
	TRequestStatus reqStat;
	this->th.Logon(reqStat);
	User::WaitForRequest(reqStat);
	this->th.Close();
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
#	ifdef DEBUG
	int res =
#	endif
			pthread_join(this->th, 0);
	ASSERT_INFO(res == 0, "res = " << strerror(res))
#else
#	error "Unsupported OS"
#endif

	//NOTE: at this point the thread's Run() method should already exit and state
	//should be set to STOPPED
	ASSERT_INFO(this->state == STOPPED, "this->state = " << this->state)

	this->state = JOINED;

//	TRACE(<< "Thread::Join(): exit" << std::endl)
}
