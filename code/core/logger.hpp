/**
 * @file
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
 * @brief GraVitoN::Logger
 *
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#ifndef _GVN_LOGGER_HEAD_
#define _GVN_LOGGER_HEAD_

#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

namespace GraVitoN
{
namespace Core
{
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// GraVitoN Logging System
namespace Logger
{

void __noLog()
{

}

#ifndef GVN_ACTIVATE_LOGGER
    #define logIt(__X__) __noLog()
    #define logItLn(__X__) __noLog()
    #define logItEndl(__X__) __noLog()
    #define logVariable(__X__,__Y__) __noLog()
#else
#ifdef GVN_ACTIVATE_LOGGER	
    // #ifdef GVN_LOG_INTO_FILE
    #ifdef GVN_LOG_FILE
		ofstream output(GVN_LOG_FILE);
	#else
		ostream &output = cout;
	#endif
#else
	ostream &output = cout;
#endif

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Print a log message
template < class _C_T_ > bool logIt ( const _C_T_ &log )
{
#ifdef GVN_ACTIVATE_LOGGER
	output << log;
    return true;
#else
    return true;
#endif
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Print an endl
bool logEndl()
{
#ifdef GVN_ACTIVATE_LOGGER
	output << endl;
    return true;
#else
    return true;
#endif
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Logger::logIt + Logger::LogEndl
template < class _C_T_ > bool logItLn ( const _C_T_ &log )
{
    logIt ( log );
    logEndl();
    return true;
}

template < class _C_T_ > bool logVariable(const string &name,const _C_T_ & value)
{
	logIt(name);
	logIt("= ");
	logItLn(value);

	return true;
}
#endif
}
}
}
#endif // _GVN_LOGGER_HEAD_

