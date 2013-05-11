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
 * @brief GraVitoN::Core::Lua
 *
*/

#ifndef _GVN_LIBLUA_HEAD_
#define _GVN_LIBLUA_HEAD_

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#ifdef OPT_LUA_52
    #include <external/liblua/lua.hpp>
    #include <external/liblua/llimits.h>
#else
    #include <external/lua.hpp>
    #include <external/lua5.1/llimits.h>

    #define LUA_OK 0
#endif

#include <graviton.hpp>
#include <core/logger.hpp>
using namespace std;

namespace GraVitoN
{
namespace Core
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class Luaviton
{
private:
    lua_State *lua_state;

    void initialize();
    bool runScript(const int prev_err = LUA_OK);

public:
    Luaviton()
    {
        lua_state = _null_;
        initialize();
    }

    bool regiserFunction (const string& func_name, const lua_CFunction func_addr );

    int loadModuleFile (const string &_file );
    int loadModuleString (const string &_script );

    int runScriptFile (const string &_file );
    int runScriptString ( const string &_script );

    void preloadModule (const string &module_name, lua_CFunction function_addr);
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo free: Double Free Exception
/*
bool free(R_Lua_Obj obj)
{
	// Double Free Exception
    if( lua_state )
	{
        lua_close ( lua_state );
		return true;
	}
	else
	{
		return false;
    }
}
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
void Luaviton::initialize()
{
    if ( lua_state == _null_ )
	{
        // General
        lua_state = luaL_newstate();
        luaL_openlibs( lua_state );

        // Lua 5.1
        // lua_state = lua_open();
        // luaopen_base(lua_state);
        // luaopen_debug(lua_state);
        // luaopen_io(lua_state);
        // luaopen_math(lua_state);
        // luaopen_package(lua_state);
        // luaopen_string(lua_state);
        // luaopen_table(lua_state);

        // Lua 5.2
        // lua_state = lua_open();
        // luaopen_base ( lua_state );
        // luaopen_coroutine ( lua_state );
        // luaopen_table ( lua_state );
        // luaopen_io ( lua_state );
        // luaopen_os(lua_state);
        // luaopen_string ( lua_state );
        // luaopen_bit32 ( lua_state );
        // luaopen_math ( lua_state );
        // // luaopen_debug ( lua_state );
        // luaopen_package ( lua_state );
	}
    //return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
//void Luaviton::printStackInfo()
//{
//    lua_Debug info;
//    int level = 0;
//    while (lua_getstack(lua_state, level, &info))
//    {
//        lua_getinfo(lua_state, "nSl", &info);
////#ifdef GVN_ACTIVATE_LOGGER
//        fprintf(stderr, "  [%d] %s:%d -- %s [%s]\n",
//            level, info.short_src, info.currentline,
//            (info.name ? info.name : "<unknown>"), info.what);
//        ++level;
////#endif
//    }
//}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Luaviton::runScript(const int prev_err)
{
    int err = LUA_OK;

	if ( prev_err != LUA_OK )
	{
        lua_pop(lua_state, 1);
		Logger::logIt("------------------ Lua Error Code: ");
		Logger::logIt(prev_err);
		Logger::logItLn(" ------------------");

        return false;
	}

    err = lua_pcall (lua_state, 0, LUA_MULTRET, 0 );

    if( err != LUA_OK )
    {
        Logger::logIt("------------------ Lua Error Code: ");
        Logger::logIt(err);
        Logger::logItLn(" ------------------");
        Logger::logItLn( lua_tostring(lua_state, -1) );
        // printStackInfo(obj);
        Logger::logItLn(" ----------------------------------------");
        // lua_pop(lua_state, 1);  /* pop error message from the stack */

        return false;
    }
    else
    {
        Logger::logItLn("------------------ Lua Executed Successfuly ------------------");
    }

    lua_pop(lua_state, 1);

    // Logger::logItLn("------------------ Lua Executed Successfuly ------------------");
	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::loadModuleFile (const string &_file )
{
    Logger::logVariable("File",_file);
    return luaL_loadfile (lua_state, _file.c_str() );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::loadModuleString (const string &_script )
{
    return luaL_loadstring ( lua_state, _script.c_str() );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::runScriptFile (const string &_file )
{
    return runScript( loadModuleFile( _file));
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::runScriptString ( const string &_script )
{
    return runScript( loadModuleString( _script));
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
void Luaviton::preloadModule (const string &module_name, lua_CFunction function_addr)
{
    lua_getfield(lua_state, LUA_GLOBALSINDEX, "package");
    lua_getfield(lua_state, -1, "preload");
    lua_pushcfunction(lua_state, function_addr);
    lua_setfield(lua_state, -2, module_name.c_str());
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Luaviton::regiserFunction (const string& func_name, const lua_CFunction func_addr )
{
    lua_register (lua_state, func_name.c_str(), func_addr );
    return true;
}

} // Core
} // GraVitoN

#endif // _GVN_LUA_HEAD_
