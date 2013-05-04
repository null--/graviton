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
#include "../WaitSet.hpp"
#include "../Ptr.hpp"
#include "../atomic.hpp"

#include "Message.hpp"
#include "Semaphore.hpp"



namespace ting{
namespace mt{



/**
 * @brief Message queue.
 * Message queue is used for communication of separate threads by
 * means of sending messages to each other. Thus, when one thread sends a message to another one,
 * it asks that another thread to execute some code portion - handler code of the message.
 * NOTE: Queue implements Waitable interface which means that it can be used in conjunction
 * with ting::WaitSet. But, note, that the implementation of the Waitable is that it
 * shall only be used to wait for READ. If you are trying to wait for WRITE the behavior will be
 * undefined.
 */
class Queue : public ting::Waitable{
	Semaphore sem;

	atomic::SpinLock mut;

	ting::Inited<Message*, 0> first;
	ting::Inited<Message*, 0> last;

#if M_OS == M_OS_WINDOWS
	//use Event to implement Waitable on Windows
	HANDLE eventForWaitable;
#elif M_OS == M_OS_MACOSX || defined(__ANDROID__) //TODO: for Android revert to using eventFD when it becomes available in Android NDK
	//use pipe to implement Waitable in *nix systems
	int pipeEnds[2];
#elif M_OS == M_OS_LINUX
	//use eventfd()
	int eventFD;
#else
#	error "Unsupported OS"
#endif

	//forbid copying
	Queue(const Queue&);
	Queue& operator=(const Queue&);

public:
	/**
	 * @brief Constructor, creates empty message queue.
	 */
	Queue();

	
	/**
	 * @brief Destructor.
	 * When called, it also destroys all messages on the queue.
	 */
	~Queue()throw();



	/**
	 * @brief Pushes a new message to the queue.
	 * @param msg - the message to push into the queue.
	 */
	void PushMessage(Ptr<Message> msg)throw();



	/**
	 * @brief Get message from queue, does not block if no messages queued.
	 * This method gets a message from message queue. If there are no messages on the queue
	 * it will return invalid auto pointer.
	 * @return auto-pointer to Message instance.
	 * @return invalid auto-pointer if there are no messages in the queue.
	 */
	Ptr<Message> PeekMsg();



	/**
	 * @brief Get message from queue, blocks if no messages queued.
	 * This method gets a message from message queue. If there are no messages on the queue
	 * it will wait until any message is posted to the queue.
	 * Note, that this method, due to its implementation, is not intended to be called from
	 * multiple threads simultaneously (unlike Queue::PeekMsg()).
	 * If it is called from multiple threads the behavior will be undefined.
	 * It is also forbidden to call GetMsg() from one thread and PeekMsg() from another
	 * thread on the same Queue instance, because it will also lead to undefined behavior.
	 * @return auto-pointer to Message instance.
	 */
	Ptr<Message> GetMsg();

private:
#if M_OS == M_OS_WINDOWS
	//override
	HANDLE GetHandle();

	u32 flagsMask;//flags to wait for

	//override
	virtual void SetWaitingEvents(u32 flagsToWaitFor);

	//returns true if signaled
	//override
	virtual bool CheckSignaled();

#elif M_OS == M_OS_LINUX
	//override
	int GetHandle();

#elif M_OS == M_OS_MACOSX //Mac OS
	//override
	int GetHandle();

#else
#	error "Unsupported OS"
#endif
};//~class Queue



}//~namespace
}//~namespace
