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
 * @brief GraVitoN::Core::Thread
 * @todo osx/android/solaris
*/

#ifndef GRAVITON_THREAD_H
#define GRAVITON_THREAD_H

#include <graviton.hpp>

#if !defined(INFO_OS_WINDOWS)
    #include <pthread.h>
#endif


namespace GraVitoN
{
    namespace Core
    {
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        /// Thread Class
        class Thread: public Core::Component
        {
        private:
#ifdef INFO_OS_WINDOWS
            static unsigned long internalCall(void *arg);
#else
            static void * internalCall(void *arg);
#endif  

        protected:
            /// Stop flag
            bool flag_stop;
#if defined(INFO_OS_WINDOWS)
            HANDLE thread;
#else
            pthread_t *thread;
#endif
            virtual bool main() = 0;
            
        public:
            /// Constructor
            Thread();

            /// Destructor
            ~Thread();

            /// thread main loop
            bool run();

            /// check if thread is still working
            virtual bool isActive();
            
            /// Wait for thread to exit
            virtual bool wait();

            /// stop thread
            virtual void stop();
        };

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Thread::Thread()
        {
            thread = _null_;
            flag_stop = true;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Thread::~Thread()
        {
            Thread::stop();
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Thread::run()
        {
            if( isActive() )
                // throw (std::logic_error("Thread::run - Thread is still running"));
                return false;
            
#ifdef INFO_OS_WINDOWS
            DWORD id;
			thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)internalCall, this, 0, &id);
            flag_stop = (thread == _null_);
#else
            thread = new pthread_t;
            
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
            int res = pthread_create(thread, &attr, internalCall, this);
            pthread_attr_destroy(&attr);

            // cout << "res: " << res << endl;
            
            if( res != 0 )
            {
                thread = _null_;
            }
            
            flag_stop = (res != 0);
#endif
            return !flag_stop;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifdef INFO_OS_WINDOWS
        unsigned long Thread::internalCall(void *arg)
#else
        void * Thread::internalCall(void *arg)
#endif
        {
            Thread *_this = (Thread*)arg;
            _this->flag_stop = false;

            _this->main();
            
            _this->flag_stop = true;

#ifdef INFO_OS_WINDOWS
            CloseHandle(_this->thread);
            _this->thread = _null_;
#else
            delete _this->thread;
            _this->thread = _null_;
#endif
            return 0;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Thread::isActive()
        {
            // cout << flag_stop << thread << endl;
            return (!flag_stop && (thread != _null_));
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Thread::wait()
        {
            if( !isActive() )
                // throw( std::logic_error("Thread::wait Thread is not running") );
                return false;

            bool res;
#ifdef INFO_OS_WINDOWS
            res = (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0);
#else
            res = (pthread_join(*thread, _null_) == 0);
#endif
            thread = _null_;
            flag_stop = true;

            return res;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        void Thread::stop()
        {
            if( !isActive() )
                // throw( std::logic_error("Thread::stop Thread is not running"));
                return;

#ifdef INFO_OS_WINDOWS
            // SuspendThread(thread);
            // Core::sleep(1);
            TerminateThread(thread, 0);
            CloseHandle(thread);
#else
            pthread_cancel(*thread);
#endif
            // cout << "------------------ STOPPED ---------------------" << endl;
            thread = _null_;
            flag_stop = true;
        }
    } /// end of Core

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
            virtual bool main()
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
                    Core::sleep(50);
                    return res;
                }

            virtual bool isActive()
                {
                    return Core::Thread::isActive();
                }

            virtual void stop()
                {
                    return Core::Thread::stop();
                }

            ~LuaThread() throw()
                {
                    Core::Thread::stop();
                }
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        void addClass_Thread()
        {
            Core::Logger::logItLn("Loaing GraVitoN.Core.Thread");

            luabridge::getGlobalNamespace ( Core::Luaviton::getInstance().getState() )
                .beginNamespace("GraVitoN")
                .beginNamespace("Core")
                .addFunction("sleep", &Core::sleep)
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

} /// end of GraVitoN

#endif /// GRAVITON_THREAD_H
