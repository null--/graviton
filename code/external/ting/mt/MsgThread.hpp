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

#include "Thread.hpp"
#include "Queue.hpp"



namespace ting{
namespace mt{



class QuitMessage;



/**
 * @brief a thread with message queue.
 * This is just a facility class which already contains message queue and boolean 'quit' flag.
 */
class MsgThread : public Thread{
	friend class QuitMessage;
	
protected:
	/**
	 * @brief Flag indicating that the thread should exit.
	 * This is a flag used to stop thread execution. The implementor of
	 * Thread::Run() method usually would want to use this flag as indicator
	 * of thread exit request. If this flag is set to true then the thread is requested to exit.
	 * The typical usage of the flag is as follows:
	 * @code
	 * class MyThread : public ting::MsgThread{
	 *     ...
	 *     void MyThread::Run(){
	 *         while(!this->quitFlag){
	 *             //get and handle thread messages, etc.
	 *             ...
	 *         }
	 *     }
	 *     ...
	 * };
	 * @endcode
	 */
	ting::Inited<volatile bool, false> quitFlag;//looks like it is not necessary to protect this flag by mutex, volatile will be enough

	Queue queue;
	
	ting::Ptr<QuitMessage> quitMessage;

public:
	inline MsgThread();

	
	
	/**
	 * @brief Send preallocated 'Quit' message to thread's queue.
	 * This function throws no exceptions. It can send the quit message only once.
	 */
	void PushPreallocatedQuitMessage()throw();
	
	
	
	/**
	 * @brief Send 'Quit' message to thread's queue.
	 */
	inline void PushQuitMessage();



	/**
	 * @brief Send "no operation" message to thread's queue.
	 */
	inline void PushNopMessage(){
		this->PushMessage(Ptr<Message>(new NopMessage()));
	}



	/**
	 * @brief Send a message to thread's queue.
	 * @param msg - a message to send.
	 */
	inline void PushMessage(Ptr<Message> msg)throw(){
		this->queue.PushMessage(msg);
	}
};



/**
 * @brief Tells the thread that it should quit its execution.
 * The handler of this message sets the quit flag (Thread::quitFlag)
 * of the thread which this message is sent to.
 */
class QuitMessage : public Message{
	MsgThread *thr;
public:
	QuitMessage(MsgThread* thread) :
			thr(thread)
	{
		if(!this->thr){
			throw ting::Exc("QuitMessage::QuitMessage(): thread pointer passed is 0");
		}
	}

	//override
	void Handle(){
		this->thr->quitFlag = true;
	}
};



inline MsgThread::MsgThread() :
		quitMessage(new QuitMessage(this))
{}



inline void MsgThread::PushQuitMessage(){
	this->PushMessage(Ptr<Message>(new QuitMessage(this)));
}



}//~namespace ting
}//~namespace
