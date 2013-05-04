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

#include "../types.hpp"



namespace ting{
namespace mt{



/**
 * @brief Message abstract class.
 * The messages are sent to message queues (see ting::mt::Queue). One message instance cannot be sent to
 * two or more message queues, only to a single queue. When sent, the message is
 * further owned by the queue (note the usage of ting::Ptr auto-pointers in the Queue class).
 */
class Message{
	friend class Queue;

	ting::Inited<Message*, 0> next;//pointer to the next message in a single-linked list

protected:
	Message()throw(){}

public:
	virtual ~Message()throw(){}

	/**
	 * @brief message handler function.
	 * This virtual method is called to handle the message. When deriving from ting::Message,
	 * override this method to define the message handler procedure.
	 */
	virtual void Handle() = 0;
};



/**
 * @brief No operation message.
 * The handler of this message does nothing when the message is handled. This message
 * can be used to unblock thread which is waiting infinitely on its message queue.
 */
class NopMessage : public Message{
public:
	NopMessage(){}

	//override
	void Handle(){
		//Do nothing
	}
};



}//~namespace
}//~namespace
