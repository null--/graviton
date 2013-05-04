#include "Queue.hpp"


#if defined(__ANDROID__)
//TODO: revert to using sys/eventfd.h when it becomes available in Android NDK
#elif M_OS == M_OS_LINUX
#		include <sys/eventfd.h>
#endif


//#define M_ENABLE_QUEUE_TRACE
#ifdef M_ENABLE_QUEUE_TRACE
#	define M_QUEUE_TRACE(x) TRACE(<< "[QUEUE] ") TRACE(x)
#else
#	define M_QUEUE_TRACE(x)
#endif



using namespace ting::mt;



Queue::Queue(){
	//can write will always be set because it is always possible to post a message to the queue
	this->SetCanWriteFlag();

#if M_OS == M_OS_WINDOWS
	this->eventForWaitable = CreateEvent(
			NULL, //security attributes
			TRUE, //manual-reset
			FALSE, //not signalled initially
			NULL //no name
		);
	if(this->eventForWaitable == NULL){
		throw ting::Exc("Queue::Queue(): could not create event (Win32) for implementing Waitable");
	}
#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
	if(::pipe(&this->pipeEnds[0]) < 0){
		std::stringstream ss;
		ss << "Queue::Queue(): could not create pipe (*nix) for implementing Waitable,"
				<< " error code = " << errno << ": " << strerror(errno);
		throw ting::Exc(ss.str().c_str());
	}
#elif M_OS == M_OS_LINUX
	this->eventFD = eventfd(0, EFD_NONBLOCK);
	if(this->eventFD < 0){
		std::stringstream ss;
		ss << "Queue::Queue(): could not create eventfd (linux) for implementing Waitable,"
				<< " error code = " << errno << ": " << strerror(errno);
		throw ting::Exc(ss.str().c_str());
	}
#else
#	error "Unsupported OS"
#endif
}



Queue::~Queue()throw(){
	//destroy messages which are currently on the queue
	{
		atomic::SpinLock::GuardYield mutexGuard(this->mut);
		Message *msg = this->first;
		Message	*nextMsg;
		while(msg){
			nextMsg = msg->next;
			//use Ptr to kill messages instead of "delete msg;" because
			//the messages are passed to PushMessage() as Ptr, and thus, it is better
			//to use Ptr to delete them.
			{
				Ptr<Message> killer(msg);
			}

			msg = nextMsg;
		}
	}
#if M_OS == M_OS_WINDOWS
	CloseHandle(this->eventForWaitable);
#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
	close(this->pipeEnds[0]);
	close(this->pipeEnds[1]);
#elif M_OS == M_OS_LINUX
	close(this->eventFD);
#else
#	error "Unsupported OS"
#endif
}



void Queue::PushMessage(Ptr<Message> msg)throw(){
	ASSERT(msg.IsValid())
	atomic::SpinLock::GuardYield mutexGuard(this->mut);
	if(this->first){
		ASSERT(this->last && this->last->next == 0)
		this->last = this->last->next = msg.Extract();
		ASSERT(this->last->next == 0)
	}else{
		ASSERT(msg.IsValid())
		this->last = this->first = msg.Extract();

		//Set CanRead flag.
		//NOTE: in linux implementation with epoll(), the CanRead
		//flag will also be set in WaitSet::Wait() method.
		//NOTE: set CanRead flag before event notification/pipe write, because
		//if do it after then some other thread which was waiting on the WaitSet
		//may read the CanRead flag while it was not set yet.
		ASSERT(!this->CanRead())
		this->SetCanReadFlag();

#if M_OS == M_OS_WINDOWS
		if(SetEvent(this->eventForWaitable) == 0){
			ASSERT(false)
		}
#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
		{
			u8 oneByteBuf[1];
			if(write(this->pipeEnds[1], oneByteBuf, 1) != 1){
				ASSERT(false)
			}
		}
#elif M_OS == M_OS_LINUX
		if(eventfd_write(this->eventFD, 1) < 0){
			ASSERT(false)
		}
#else
#	error "Unsupported OS"
#endif
	}

	ASSERT(this->CanRead())
	//NOTE: must do signaling while mutex is locked!!!
	this->sem.Signal();
}



ting::Ptr<Message> Queue::PeekMsg(){
	atomic::SpinLock::GuardYield mutexGuard(this->mut);
	if(this->first){
		ASSERT(this->CanRead())
		//NOTE: Decrement semaphore value, because we take one message from queue.
		//      The semaphore value should be > 0 here, so there will be no hang
		//      in Wait().
		//      The semaphore value actually reflects the number of Messages in
		//      the queue.
		this->sem.Wait();

		ASSERT(this->first)
		if(this->first->next == 0){//if we are taking away the last message from the queue
#if M_OS == M_OS_WINDOWS
			if(ResetEvent(this->eventForWaitable) == 0){
				ASSERT(false)
				throw ting::Exc("Queue::Wait(): ResetEvent() failed");
			}
#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
			{
				u8 oneByteBuf[1];
				if(read(this->pipeEnds[0], oneByteBuf, 1) != 1){
					throw ting::Exc("Queue::Wait(): read() failed");
				}
			}
#elif M_OS == M_OS_LINUX
			{
				eventfd_t value;
				if(eventfd_read(this->eventFD, &value) < 0){
					throw ting::Exc("Queue::Wait(): eventfd_read() failed");
				}
				ASSERT(value == 1)
			}
#else
#	error "Unsupported OS"
#endif
			this->ClearCanReadFlag();
		}else{
			ASSERT(this->CanRead())
		}

		Message* ret = this->first;
		this->first = this->first->next;
		
		return Ptr<Message>(ret);
	}
	return Ptr<Message>();
}



ting::Ptr<Message> Queue::GetMsg(){
	M_QUEUE_TRACE(<< "Queue[" << this << "]::GetMsg(): enter" << std::endl)
	{
		atomic::SpinLock::GuardYield mutexGuard(this->mut);
		if(this->first){
			ASSERT(this->CanRead())
			//NOTE: Decrement semaphore value, because we take one message from queue.
			//      The semaphore value should be > 0 here, so there will be no hang
			//      in Wait().
			//      The semaphore value actually reflects the number of Messages in
			//      the queue.
			this->sem.Wait();

			ASSERT(this->first)
			
			if(this->first->next == 0){//if we are taking away the last message from the queue
#if M_OS == M_OS_WINDOWS
				if(ResetEvent(this->eventForWaitable) == 0){
					ASSERT(false)
					throw ting::Exc("Queue::Wait(): ResetEvent() failed");
				}
#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
				{
					u8 oneByteBuf[1];
					read(this->pipeEnds[0], oneByteBuf, 1);
				}
#elif M_OS == M_OS_LINUX
				{
					eventfd_t value;
					if(eventfd_read(this->eventFD, &value) < 0){
						throw ting::Exc("Queue::Wait(): eventfd_read() failed");
					}
					ASSERT(value == 1)
				}
#else
#	error "Unsupported OS"
#endif
				this->ClearCanReadFlag();
			}else{
				ASSERT(this->CanRead())
			}
			
			Message* ret = this->first;
			this->first = this->first->next;

			M_QUEUE_TRACE(<< "Queue[" << this << "]::GetMsg(): exit without waiting on semaphore" << std::endl)
			return Ptr<Message>(ret);
		}
	}
	M_QUEUE_TRACE(<< "Queue[" << this << "]::GetMsg(): waiting" << std::endl)

	this->sem.Wait();

	M_QUEUE_TRACE(<< "Queue[" << this << "]::GetMsg(): signaled" << std::endl)
	{
		atomic::SpinLock::GuardYield mutexGuard(this->mut);
		ASSERT(this->CanRead())
		ASSERT(this->first)

		if(this->first->next == 0){//if we are taking away the last message from the queue
#if M_OS == M_OS_WINDOWS
			if(ResetEvent(this->eventForWaitable) == 0){
				ASSERT(false)
				throw ting::Exc("Queue::Wait(): ResetEvent() failed");
			}
#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
			{
				u8 oneByteBuf[1];
				read(this->pipeEnds[0], oneByteBuf, 1);
			}
#elif M_OS == M_OS_LINUX
			{
				eventfd_t value;
				if(eventfd_read(this->eventFD, &value) < 0){
					throw ting::Exc("Queue::Wait(): eventfd_read() failed");
				}
				ASSERT(value == 1)
			}
#else
#	error "Unsupported OS"
#endif
			this->ClearCanReadFlag();
		}else{
			ASSERT(this->CanRead())
		}
		
		Message* ret = this->first;
		this->first = this->first->next;

		M_QUEUE_TRACE(<< "Queue[" << this << "]::GetMsg(): exit after waiting on semaphore" << std::endl)
		return Ptr<Message>(ret);
	}
}



#if M_OS == M_OS_WINDOWS
//override
HANDLE Queue::GetHandle(){
	//return event handle
	return this->eventForWaitable;
}



//override
void Queue::SetWaitingEvents(u32 flagsToWaitFor){
	//It is not allowed to wait on queue for write,
	//because it is always possible to push new message to queue.
	//Error condition is not possible for Queue.
	//Thus, only possible flag values are READ and 0 (NOT_READY)
	if(flagsToWaitFor != 0 && flagsToWaitFor != ting::Waitable::READ){
		ASSERT_INFO(false, "flagsToWaitFor = " << flagsToWaitFor)
		throw ting::Exc("Queue::SetWaitingEvents(): flagsToWaitFor should be ting::Waitable::READ or 0, other values are not allowed");
	}

	this->flagsMask = flagsToWaitFor;
}



//returns true if signaled
//override
bool Queue::CheckSignaled(){
	//error condition is not possible for queue
	ASSERT((this->readinessFlags & ting::Waitable::ERROR_CONDITION) == 0)

/*
#ifdef DEBUG
	{
		atomic::SpinLock::GuardYield mutexGuard(this->mut);
		if(this->first){
			ASSERT_ALWAYS(this->CanRead())

			//event should be in signalled state
			ASSERT_ALWAYS(WaitForSingleObject(this->eventForWaitable, 0) == WAIT_OBJECT_0)
		}

		if(this->CanRead()){
			ASSERT_ALWAYS(this->first)

			//event should be in signalled state
			ASSERT_ALWAYS(WaitForSingleObject(this->eventForWaitable, 0) == WAIT_OBJECT_0)
		}

		//if event is in signalled state
		if(WaitForSingleObject(this->eventForWaitable, 0) == WAIT_OBJECT_0){
			ASSERT_ALWAYS(this->CanRead())
			ASSERT_ALWAYS(this->first)
		}
	}
#endif
*/

	return (this->readinessFlags & this->flagsMask) != 0;
}

#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
//override
int Queue::GetHandle(){
	//return read end of pipe
	return this->pipeEnds[0];
}

#elif M_OS == M_OS_LINUX
//override
int Queue::GetHandle(){
	return this->eventFD;
}

#else
#	error "Unsupported OS"
#endif
