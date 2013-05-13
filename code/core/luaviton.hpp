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
    // #include <external/liblua/llimits.h>
#else
    #include <external/lua.hpp>
    // #include <external/lua5.1/llimits.h>

    #define LUA_OK 0
#endif

#include <graviton.hpp>
#include <core/component.hpp>
#include <core/logger.hpp>

#include <external/LuaBridge/LuaBridge.h>
#include <external/LuaBridge/RefCountedPtr.h>

using namespace std;

namespace GraVitoN
{
namespace Core
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class Luaviton : public Component_Singleton<Luaviton>
{
private:
    lua_State *lua_state;

    bool runScript(const int prev_err = LUA_OK);
    void printErrorCode (const int err);

    // static Luaviton *instance;

protected:
    void initialize();
    static int traceback(lua_State *L);

public:
    Luaviton()
    {
        lua_state = _null_;
        initialize();
    }

    lua_State * getState()
    {
        return lua_state;
    }

//    static Luaviton * getInstance()
//    {
//        if( !instance )
//            instance = new Luaviton();
//        return instance;
//    }

    ~Luaviton();
    bool regiserFunction (const string &func_name, const lua_CFunction func_addr );
    void callLuaFunction (const string &lua_func_name, int nparams, int nreurns);

    int loadModuleFile (const string &_file );
    int loadModuleString (const string &_script );

    int runScriptFile (const string &_file );
    int runScriptString ( const string &_script );

    void preloadModule (const string &module_name, lua_CFunction function_addr);
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Beautiful!
template <class _CS_MOD_> class Luaviton_Module : public Component_Singleton<_CS_MOD_>
{
protected:
    Luaviton &luaviton;

public:
    Luaviton_Module() : luaviton(Luaviton::getInstance()) {}
    virtual ~Luaviton_Module() {}

    virtual void registerModule() = 0;
    virtual void loadEmAll() = 0;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Luaviton::~Luaviton()
{
    // Double Free Exception
    if( lua_state )
    {
        lua_close ( lua_state );
    }
}

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

    /// add traceback
    /// @todo need more tests
    lua_pushcfunction (lua_state, &traceback);
    luaL_ref (lua_state, LUA_REGISTRYINDEX);

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
int Luaviton::traceback (lua_State *L)
{
    // look up Lua's 'debug.traceback' function
    lua_getglobal(L, "debug");
    if (!lua_istable(L, -1))
    {
        lua_pop(L, 1);
        return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 2);
        return 1;
    }
    lua_pushvalue(L, 1);  /* pass error message */
    lua_pushinteger(L, 2);  /* skip this function and traceback */
    lua_call(L, 2, 1);  /* call debug.traceback */
    return 1;
}

void Luaviton::printErrorCode(const int err)
{
    GraVitoN::Core::Logger::logItLn("---------------------------------------------------------");
    switch (err)
    {
        case LUA_OK:
            GraVitoN::Core::Logger::logItLn("[LUA] Success"); break;
        case LUA_ERRRUN:
            GraVitoN::Core::Logger::logItLn("[LUA ERROR] Runtime error"); break;
        case LUA_ERRSYNTAX:
            GraVitoN::Core::Logger::logItLn("[LUA ERROR] Syntax error"); break;
        case LUA_ERRERR:
            GraVitoN::Core::Logger::logItLn("[LUA ERROR] Error while trying to print an error."); break;
        case LUA_ERRFILE:
            GraVitoN::Core::Logger::logItLn("[LUA ERROR] Couldn't open or read file"); break;
        case LUA_ERRMEM:
            GraVitoN::Core::Logger::logItLn("[LUA ERROR] Memory allocation error"); break;
        default:
            GraVitoN::Core::Logger::logVariable("[LUA ERROR] CODE: ", err); break;
    }

    if( err != LUA_OK ) Logger::logItLn( lua_tostring(lua_state, -1) );
    GraVitoN::Core::Logger::logItLn("---------------------------------------------------------");
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool Luaviton::runScript(const int prev_err)
{
    // Check any previous error
    int err = LUA_OK;

    if ( prev_err != LUA_OK )
    {
        lua_pop(lua_state, 1);
        printErrorCode(prev_err);
        return false;
    }

    err = lua_pcall (lua_state, 0, LUA_MULTRET, 0 );

    printErrorCode(err);

    if( err != LUA_OK )
    {
        return false;
    }

    lua_pop(lua_state, 1);
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::loadModuleFile (const string &_file )
{
    Logger::logVariable("File",_file);
    return luaL_dofile (lua_state, _file.c_str() );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::loadModuleString (const string &_script )
{
    return luaL_dostring ( lua_state, _script.c_str() );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::runScriptFile (const string &_file )
{
    // return runScript( luaL_dofile(lua_state, _file.c_str()) );
    return runScript( luaL_loadfile(lua_state, _file.c_str()) );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int Luaviton::runScriptString ( const string &_script )
{
    return runScript( luaL_loadstring(lua_state, _script.c_str()));
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
