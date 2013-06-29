/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
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
 * @brief GraVitoN multi-Threaded Component
 *
 */

#ifndef GRAVITON_COMPONENT_PARALLEL_H
#define GRAVITON_COMPONENT_PARALLEL_H

#include <core/component.hpp>
#include <core/logger.hpp>
#include <core/thread.hpp>

#include <list>
#include <string>
#include <cstring>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// GraVitoN Master Nemspace
namespace GraVitoN
{

    namespace Utils
    {

/**
 * @brief Parallel Component
 * If you want to run a group of components simultaneously, Use this class
 *
 * @note
 * Construction and Destruction of the Object is up to programmer
 */
        class Component_Parallel
        {
        private:
            /// Run a component inside a thread
            class Component_Thread : public GraVitoN::Core::Thread
            {
            private:
                Core::Component &c_handle;

            protected:
                bool main()
                    {
                        try
                        {
                            if( c_handle.run() )
                                return false;
                        }
                        catch(...)
                        {
                            Core::Logger::logItLn("Thread::MainLoop Exceptions");
                        }

                        return true;
                    }

            public:
                Component_Thread(Core::Component &_valid_component):
                    c_handle(_valid_component)
                    {
                    }

                virtual ~Component_Thread()throw()
                    {

                    }
            };

            list<Component_Thread*> internal_threads;

        public:
            Component_Parallel();
            ~Component_Parallel();

            /// Run a component on a new thread
            bool runThis(GraVitoN::Core::Component &component);

            /// Is there any active thread?
            bool isActive();

            /// Stop all threads
            bool stopAll();
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Component_Parallel::Component_Parallel()
        {

        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Component_Parallel::~Component_Parallel()
        {
            stopAll();
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Component_Parallel::stopAll()
        {
            try
            {
                for(list<Component_Thread*>::iterator p_trd = internal_threads.begin(); p_trd != internal_threads.end(); ++p_trd)
                    (*p_trd)->stop();
                internal_threads.clear();
            }
            catch(...)
            {
                return false;
            }
            return true;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Component_Parallel::runThis(GraVitoN::Core::Component &component)
        {
            internal_threads.push_back( new Component_Thread(component) );
            //internal_threads.back()->initialize();
            return internal_threads.back()->run();
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Component_Parallel::isActive()
        {
            for(list<Component_Thread*>::iterator p_trd = internal_threads.begin(); p_trd != internal_threads.end(); ++p_trd)
                if ( (*p_trd)->isActive() )
                    return true;
            return false;
        }

    }
}

#endif /// end of GRAVITON_COMPONENT_PARALLEL
