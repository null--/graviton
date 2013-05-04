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
 * @brief GraVitoN Threaded Component
 *
*/

#ifndef _GVN_COMPONENT_PARALLEL_HEAD_
#define _GVN_COMPONENT_PARALLEL_HEAD_

#include "gvn_component.hpp"
#include "gvn_utils/gvn_logger.hpp"
#include "gvn_utils/gvn_thread.hpp"
#include <list>
#include <string>
#include <cstring>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// GraVitoN Master Nemspace
namespace GraVitoN
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
	class Component_Thread : public GraVitoN::Thread
	{
	private:
		Component *c_handle;
		std::string c_options;

	protected:
		bool myMainLoop()
		{
			try
			{
				if( c_handle->run() )
					return false;
			}
			catch(...)
			{
				Logger::logItLn("Thread::MainLoop Exceptions");
			}

			return true;
		}

	public:
		Component_Thread(Component *_valid_component_pointer, const string &_options)
		{
			c_handle = _valid_component_pointer;
			c_handle->initialize(_options);
		}

		virtual ~Component_Thread()throw()
		{

		}
	};

	list<Component_Thread*> internal_threads;

public:
	Component_Parallel();
	~Component_Parallel();

	/// Run this component on a new thread
	bool runThis(Component *component, const string &options);

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

bool Component_Parallel::runThis(Component *component, const string &options)
{
	internal_threads.push_back( new Component_Thread(component, options) );
	internal_threads.back()->initialize();
	internal_threads.back()->run();
}

bool Component_Parallel::isActive()
{
	for(list<Component_Thread*>::iterator p_trd = internal_threads.begin(); p_trd != internal_threads.end(); ++p_trd)
		if ( (*p_trd)->isActive() )
			return true;
	return false;
}

}

#endif // _GVN_COMPONENT_PARALLEL_HEAD_
