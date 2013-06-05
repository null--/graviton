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
*/

#ifndef _GVN_THREAD_HEAD_
#define _GVN_THREAD_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <external/ting/net/Lib.hpp>
#include <external/ting/mt/MsgThread.hpp>

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

    virtual bool myMainLoop() = 0;
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
typedef void *(*THREAD_FUNC_P) (void *);
class QuickThread : public Thread
{
private:
    THREAD_FUNC_P loop_func_addr;
    void *args;

protected:
    virtual bool myMainLoop()
    {
        if(!loop_func_addr)
            return false;
        loop_func_addr( args );
        return true;
    }

public:
    QuickThread(THREAD_FUNC_P loop_function, void *loop_args)
    {
        loop_func_addr = loop_function;
        args = loop_args;
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class Semaphore: private ting::mt::Semaphore
{
public:
    Semaphore(const unsigned init_val = 0):ting::mt::Semaphore(init_val){}

    void wait(const unsigned int milisec = 0)
    {
        if(milisec == 0)
            ting::mt::Semaphore::Wait();
        else
            ting::mt::Semaphore::Wait(milisec);
    }

    void signal()
    {
        ting::mt::Semaphore::Signal();
    }
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
    flag_stop = false;

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
/*
bool Thread::myMainLoop()
{
    if( !flag_stop )
        Logger::logItLn("[Thread] I'm Running!");
    return true;
}
*/

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

} // end of Core

//=============================================================================//
#ifdef GVN_ACTIVATE_LUABRIDGE
namespace LUABridge
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class LuaThread : public Core::Thread
{
private:
    luabridge::LuaRef loop_func_addr;

protected:
    virtual bool myMainLoop()
    {
        loop_func_addr();
        return true;
    }

public:
    LuaThread(luabridge::LuaRef callback) : loop_func_addr(callback)
    {
    }

    virtual bool run()
    {

        bool res =Core::Thread::run();
        /// Avoid segmentation fault at lua
        Core::Thread::sleep(50);
        return res;
    }

    virtual bool isActive()
    {
        return Core::Thread::isActive();
    }

    virtual bool stop()
    {
        return Core::Thread::stop();
    }

    ~LuaThread() throw()
    {
        Core::Thread::stop();
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo
void addClass_Thread()
{
    luabridge::getGlobalNamespace ( Core::Luaviton::getInstance().getState() )
            .beginNamespace("GraVitoN")
            .beginNamespace("Core")
            .addFunction("sleep", &Core::Thread::sleep)
            .beginClass <LuaThread> ("Thread")
            .addConstructor < void(*) (luabridge::LuaRef), RefCountedPtr<LuaThread> > ()
            .addFunction("run", &LuaThread::run)
            .addFunction("isActive", &LuaThread::isActive)
            .addFunction("stop", &LuaThread::stop)
            .endClass()
            .endNamespace()
            .endNamespace()
            ;
}
}
#endif

} // end of GraVitoN

#endif // _GVN_THREAD_HEAD_
