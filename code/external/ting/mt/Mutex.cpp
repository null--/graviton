#include "Mutex.hpp"

#include "../Exc.hpp"


using namespace ting::mt;



Mutex::Mutex(){
//	TRACE(<< "Mutex::Mutex(): invoked " << this << std::endl)
#if M_OS == M_OS_WINDOWS
	InitializeCriticalSection(&this->m);
#elif M_OS == M_OS_SYMBIAN
	if(this->m.CreateLocal() != KErrNone){
		throw ting::Exc("Mutex::Mutex(): failed creating mutex (CreateLocal() failed)");
	}
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
	if(pthread_mutex_init(&this->m, NULL) != 0){
		throw ting::Exc("Mutex::Mutex(): failed creating mutex (pthread_mutex_init() failed)");
	}
#else
#	error "unknown OS"
#endif
}



Mutex::~Mutex()throw(){
//	TRACE(<< "Mutex::~Mutex(): invoked " << this << std::endl)
#if M_OS == M_OS_WINDOWS
	DeleteCriticalSection(&this->m);
#elif M_OS == M_OS_SYMBIAN
	this->m.Close();
#elif M_OS == M_OS_LINUX || M_OS == M_OS_MACOSX
#	ifdef DEBUG
	int ret =
#	endif
	pthread_mutex_destroy(&this->m);
#	ifdef DEBUG
	if(ret != 0){
		std::stringstream ss;
		ss << "Mutex::~Mutex(): pthread_mutex_destroy() failed"
				<< " error code = " << ret << ": " << strerror(ret) << ".";
		if(ret == EBUSY){
			ss << " You are trying to destroy locked mutex.";
		}
		ASSERT_INFO_ALWAYS(false, ss.str())
	}
#	endif
#else
#	error "unknown OS"
#endif
}
