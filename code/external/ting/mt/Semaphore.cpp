#include "Semaphore.hpp"



using namespace ting::mt;



Semaphore::Semaphore(unsigned initialValue){
#if M_OS == M_OS_WINDOWS
	if( (this->s = CreateSemaphore(NULL, initialValue, 0xffffff, NULL)) == NULL)
#elif M_OS == M_OS_SYMBIAN
	if(this->s.CreateLocal(initialValue) != KErrNone)
#elif M_OS == M_OS_MACOSX
	// Darwin/BSD/... semaphores are named semaphores, we need to create a 
	// different name for new semaphores.
	char name[256];
	// this n_name is shared among all semaphores, maybe it will be worth protect it
	// by a mutex or a CAS operation;
	static unsigned int n_name = 0;
	unsigned char p = 0;
	// fill the name
	for(unsigned char n = ++n_name, p = 0; n > 0;){
		unsigned char idx = n%('Z'-'A'+1);
		name[++p] = 'A'+idx;
		n -= idx;
	}
	// end it with null and create the semaphore
	name[p] = '\0';
	this->s = sem_open(name, O_CREAT, SEM_VALUE_MAX, initialValue);
	if (this->s == SEM_FAILED)
#elif M_OS == M_OS_LINUX
	if(sem_init(&this->s, 0, initialValue) < 0)
#else
#	error "unknown OS"
#endif
	{
		LOG(<< "Semaphore::Semaphore(): failed" << std::endl)
		throw ting::Exc("Semaphore::Semaphore(): creating semaphore failed");
	}
}



Semaphore::~Semaphore()throw(){
#if M_OS == M_OS_WINDOWS
	CloseHandle(this->s);
#elif M_OS == M_OS_SYMBIAN
	this->s.Close();
#elif M_OS == M_OS_MACOSX
	sem_close(this->s);
#elif M_OS == M_OS_LINUX
	sem_destroy(&this->s);
#else
#	error "unknown OS"
#endif
}



bool Semaphore::Wait(ting::u32 timeoutMillis){
#if M_OS == M_OS_WINDOWS
	STATIC_ASSERT(INFINITE == 0xffffffff)
	switch(WaitForSingleObject(this->s, DWORD(timeoutMillis == INFINITE ? INFINITE - 1 : timeoutMillis))){
		case WAIT_OBJECT_0:
			return true;
		case WAIT_TIMEOUT:
			return false;
		default:
			throw ting::Exc("Semaphore::Wait(u32): wait failed");
			break;
	}
#elif M_OS == M_OS_MACOSX
	int retVal = 0;

	// simulate the behavior of wait
	do{
		retVal = sem_trywait(this->s);
		if(retVal == 0){
			break; // OK leave the loop
		}else{
			if(errno == EAGAIN){ // the semaphore was blocked
				struct timespec amount;
				struct timespec result;
				int resultsleep;
				amount.tv_sec = timeoutMillis / 1000;
				amount.tv_nsec = (timeoutMillis % 1000) * 1000000;
				resultsleep = nanosleep(&amount, &result);
				// update timeoutMillis based on the output of the sleep call
				// if nanosleep() returns -1 the sleep was interrupted and result
				// struct is updated with the remaining un-slept time.
				if(resultsleep == 0){
					timeoutMillis = 0;
				}else{
					timeoutMillis = result.tv_sec * 1000 + result.tv_nsec / 1000000;
				}
			}else if(errno != EINTR){
				throw ting::Exc("Semaphore::Wait(): wait failed");
			}
		}
	}while(timeoutMillis > 0);

	// no time left means we reached the timeout
	if(timeoutMillis == 0){
		return false;
	}
#elif M_OS == M_OS_LINUX
	//if timeoutMillis is 0 then use sem_trywait() to avoid unnecessary time calculation for sem_timedwait()
	if(timeoutMillis == 0){
		if(sem_trywait(&this->s) == -1){
			if(errno == EAGAIN){
				return false;
			}else{
				throw ting::Exc("Semaphore::Wait(u32): error: sem_trywait() failed");
			}
		}
	}else{
		timespec ts;

		if(clock_gettime(CLOCK_REALTIME, &ts) == -1)
			throw ting::Exc("Semaphore::Wait(): clock_gettime() returned error");

		ts.tv_sec += timeoutMillis / 1000;
		ts.tv_nsec += (timeoutMillis % 1000) * 1000 * 1000;
		ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
		ts.tv_nsec = ts.tv_nsec % (1000 * 1000 * 1000);

		if(sem_timedwait(&this->s, &ts) == -1){
			if(errno == ETIMEDOUT){
				return false;
			}else{
				throw ting::Exc("Semaphore::Wait(u32): error: sem_timedwait() failed");
			}
		}
	}
#else
#	error "unknown OS"
#endif
	return true;
}
