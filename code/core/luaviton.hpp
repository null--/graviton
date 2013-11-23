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
 * @brief GraVitoN::Core::Luaviton
 *
*/

#ifndef GRAVITON_LUAVITON_H
#define GRAVITON_LUAVITON_H

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

using namespace std;

namespace GraVitoN
{
    namespace Core
    {

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        /// Luaviton Class
        /// An interface for using lua features. (e.g. run lua scripts)
        class Luaviton : public Component_Singleton<Luaviton>
        {
        private:
            /// Lua state
            lua_State *lua_state;

            /// Last lua error (0 means OK)
            int last_error;

            /// run latest added lua script
            bool runScript(const int prev_err = LUA_OK);

            /// print lua errors
            void printErrorCode (const int err);

        protected:
            /// initialize luaviton
            void initialize();

            /// lua stack trace
            static int traceback(lua_State *L);

        public:
            /// Constructor
            Luaviton()
                {
                    lua_state = _null_;
                    initialize();
                }

            /// Destructor
            ~Luaviton();

            /// get lua state: USE IT CAREFULLY
            lua_State * getState()
                {
                    return lua_state;
                }

            /// Register a C++ function
            bool regiserFunction (const std::string &func_name, const lua_CFunction func_addr );

            /// Call a lua function from C++
            void callLuaFunction (const std::string &lua_func_name, int nparams, int nreurns);

            /// Load a lua module file
            bool loadModuleFile (const std::string &_file );
            /// Load a lua module written in a string variable
            bool loadModuleString (const std::string &_script );

            /// Run a lua script file
            bool runScriptFile (const std::string &_file );
            /// Run a lua script written is a string variable
            bool runScriptString ( const std::string &_script );

            /// Load C++ coded lua module
            void preloadModule (const std::string &module_name, lua_CFunction function_addr);
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
#if defined(GVN_VERBOSE_LUA) || defined(GVN_ACTIVATE_LOGGER)
            cout << "---------------------------------------------------------" << endl;
#endif
            switch (err)
            {
#if defined(GVN_VERBOSE_LUA) || defined(GVN_ACTIVATE_LOGGER)
            case LUA_OK:
                cout << "[LUA] Success" << endl; break;
            case LUA_ERRRUN:
                cout << "[LUA ERROR] Runtime error" << endl; break;
            case LUA_ERRSYNTAX:
                cout << "[LUA ERROR] Syntax error" << endl; break;
            case LUA_ERRERR:
                cout << "[LUA ERROR] Error while trying to print an error." << endl; break;
            case LUA_ERRFILE:
                cout << "[LUA ERROR] Couldn't open or read file" << endl; break;
            case LUA_ERRMEM:
                cout << "[LUA ERROR] Memory allocation error" << endl; break;
            default:
                cout << "[LUA ERROR] CODE: " << err << endl; break;
#endif
            }
#if defined(GVN_VERBOSE_LUA) || defined(GVN_ACTIVATE_LOGGER)
            if( err != LUA_OK ) cout << lua_tostring(lua_state, -1) << endl;
            cout << "---------------------------------------------------------" << endl;
#endif
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
        bool Luaviton::loadModuleFile (const std::string &_file )
        {
            Logger::logVariable("Loading LUA Module: ",_file);
            return luaL_dofile (lua_state, _file.c_str() ) == LUA_OK;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Luaviton::loadModuleString (const std::string &_script )
        {
            Logger::logItLn("Loading LUA Module (string)");
            return luaL_dostring ( lua_state, _script.c_str() ) == LUA_OK;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Luaviton::runScriptFile (const std::string &_file )
        {
            // return runScript( luaL_dofile(lua_state, _file.c_str()) );
            return runScript( luaL_loadfile(lua_state, _file.c_str()) );
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Luaviton::runScriptString ( const std::string &_script )
        {
            return runScript( luaL_loadstring(lua_state, _script.c_str()));
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        void Luaviton::preloadModule (const std::string &module_name, lua_CFunction function_addr)
        {
            lua_getfield(lua_state, LUA_GLOBALSINDEX, "package");
            lua_getfield(lua_state, -1, "preload");
            lua_pushcfunction(lua_state, function_addr);
            lua_setfield(lua_state, -2, module_name.c_str());
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Luaviton::regiserFunction (const std::string& func_name, const lua_CFunction func_addr )
        {
            lua_register (lua_state, func_name.c_str(), func_addr );
            return true;
        }

    } /// end of Core
} /// end of GraVitoN

#endif /// end of GRAVITON_LUAVITON_H
