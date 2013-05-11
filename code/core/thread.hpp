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

#include "graviton.hpp"
#include "logger.hpp"
#include "net/Lib.hpp"
#include "mt/MsgThread.hpp"

namespace GraVitoN
{

namespace Core
{

// Cross Platform OS version sleep function
/*
void os_sleep(unsigned int __msec__)
{
    ting::mt::Thread::Sleep(__msec__);
}
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Thread
class Thread: private ting::mt::MsgThread, public GraVitoN::Core::Component
{
public:
    /// Sleep for a number of milliseconds
    void static sleep(unsigned int __msec__)
    {
        // os_sleep(__msec__);
        ting::mt::Thread::Sleep(__msec__);
    }

private:
    /// Override ting::mt::Thread::Run()
    virtual void Run();

protected:
    /// Stop flag. true means you have to stop your main loop.
    bool flag_stop;

    virtual bool myMainLoop();
    //virtual bool initialize();

public:
    Thread();
    virtual ~Thread() throw();

    virtual bool run();

    virtual bool isActive();

    /// Wait For Thread To Exit (Like pthread_join)
    virtual bool waitForEndOfMainLoop();

    /// Stop thread
    virtual bool stop();
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Thread::Thread()
{
    flag_stop = true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Thread::~Thread() throw()
{
    stop();
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

    ting::Ptr<ting::mt::Message> quit( new ting::mt::QuitMessage(this) );
    this->PushMessage(quit);

    //waitForEndOfMainLoop();

    return true;
}

}
} // end of GraVitoN

#endif // _GVN_THREAD_HEAD_
