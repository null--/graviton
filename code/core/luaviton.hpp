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
 * @brief GraVitoN::Lua
 *
*/

#ifndef _GVN_LIBLUA_HEAD_
#define _GVN_LIBLUA_HEAD_

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <iostream>
#include "lib_lua/lua.hpp"
#include "lib_lua/llimits.h"
#include "../graviton.hpp"
#include "../gvn_utils/gvn_logger.hpp"
using namespace std;

namespace GraVitoN
{
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Lua interface for GraVitoN
 *
 * Sometimes our malware needs to changes it's behavior during run-time, for example imagine
 * that you need to upgrade your virus's spreeding technique (Gvn_Intercross) during a virus attack!
 * You can change a lua script instead of recompiling your viri!
 *
 */
class Lua : public Component
{
private:
    /// Lua Current State
    lua_State *lua_state;

	/// Run a loaded script
	bool runScript(const int &prev_err);

public:
    /// Constructor
	Lua();

    /// Destructor
	virtual ~Lua();

    /**
     * @brief Initialize Lua
     *
     * Setup Lua Interface for your malware
     *
     * @param [in] _options
     * Lua initializer options
     *
     * @return
     * True if Lua initialized successfully
     */
    virtual bool initialize ( const string &_options = "" );

    /**
     * @brief Run a string scripts
     *
     * Run your scripts which is written inside a string
     *
     * @param [in] _script
     * Yoyr Script
     *
	 * @return
	 * True means successful execution
     */
	virtual int runScriptString ( const string &_script );

    /**
     * @brief Run a script file
     *
     * Run your script file
     *
     * @param [in] _file
     * Script File
     *
	 * @return
	 * True means successful execution
	*/
	virtual int runScriptFile ( const string &_file );

    /**
     * @brief Register New Function
     *
     * @param [in] func_name
     * Name of your function, you want to call inside lua_CFunction
     *
     * @param [in] func_addr
     * Address of your function.\n
     * Your function is a 'int func(lua_State *L)' \n
     *
     */
    virtual bool regiserFunction ( const string &func_name, lua_CFunction func_addr );
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Lua::Lua()
{
    options = "<no_options>";

    lua_state = _null_;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Lua::~Lua()
{
	/*
	// Double Free Exception
	if( lua_state )
	{
		lua_close ( lua_state );
	}
	*/
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Lua::initialize ( const string &_options )
{
	options = _options;

	if ( lua_state == _null_ )
	{
		lua_state = luaL_newstate();

		luaL_openlibs( lua_state );
		/*
		luaopen_base ( lua_state );
		luaopen_coroutine ( lua_state );
		luaopen_table ( lua_state );
		luaopen_io ( lua_state );
		luaopen_os(lua_state);
		luaopen_string ( lua_state );
		luaopen_bit32 ( lua_state );
		luaopen_math ( lua_state );
		luaopen_debug ( lua_state );
		luaopen_package ( lua_state );
		*/
	}
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Lua::runScript(const int &prev_err)
{
	if ( prev_err != LUA_OK )
	{
		lua_pop(lua_state, 1);
		Logger::logIt("------------------ Lua Error Code: ");
		Logger::logIt(prev_err);
		Logger::logItLn(" ------------------");
		return false;
	}

	int err = lua_pcall ( lua_state, 0, LUA_MULTRET, 0 );
	lua_pop(lua_state, 1);

	if( err != LUA_OK )
	{
		Logger::logIt("------------------ Lua Error Code: ");
		Logger::logIt(err);
		Logger::logItLn(" ------------------");
		return false;
	}
	else
	{
		Logger::logItLn("------------------ Lua Executed Successfuly ------------------");
	}

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Lua::runScriptFile ( const string &_file )
{
	Logger::logVariable("File",_file);
	int err = luaL_loadfile (lua_state, _file.c_str() );
	return runScript(err);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Lua::runScriptString ( const string &_script )
{
    int err = luaL_loadstring ( lua_state, _script.c_str() );
	return runScript(err);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Lua::regiserFunction ( const string& func_name, const lua_CFunction func_addr )
{
    lua_register ( lua_state, func_name.c_str(), func_addr );
    return true;
}

}

#endif // _GVN_LUA_HEAD_
