/*
  +-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
  | This file is part of Graviton.                                       |
  |                                                                      |
  | Graviton is free software: you can redistribute it and/or modify     |
  | it under the terms of the GNU General Public License as published by |
  | the Free Software Foundation, either version 3 of the License, or    |
  | (at your option) any later version.                                  |
  |                                                                      |
  | Graviton is distributed in the hope that it will be useful,          |
  | but WITHOUT ANY WARRANTY; without even the implied warranty of       |
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        |
  | GNU General Public License for more details.                         |
  |                                                                      |
  | You should have received a copy of the GNU General Public License    |
  | along with Graviton.  If not, see <http://www.gnu.org/licenses/>.    |
  +-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
*/

/*
 * Programmers:
 * 		1- Sina Hatef Matbue | _null_ | sinahatef.cpp@gmail.com
 */

#ifndef _GVN_LUA_HEAD_
#define _GVN_LUA_HEAD_

#include <iostream>
#include "lua_lib/lua.hpp"
#include "../graviton.hpp"
#include "../gvn_utils/gvn_logger.hpp"

class Gvn_Lua : public Gvn_Component
{
private:
    lua_State *lua_state;

public:
    Gvn_Lua();
    virtual ~Gvn_Lua();

    virtual int initialize ( const string &_options = "" );
    virtual int runScriptString ( const string &_script );
    virtual int runScriptFile ( const string &_file );
};

Gvn_Lua::Gvn_Lua()
{
    options = "<no_options>";
    info.about = "Lua Component For GraVitoN";
    info.architecture = "multi-art";
    info.hardware = "multi-hw";
    info.name = "Lua for GraVitoN";
    info.os = "multi-os";
    info.version = "5.2.0";

    lua_state = _null_;
}

Gvn_Lua::~Gvn_Lua()
{
    lua_close ( lua_state );
}


int Gvn_Lua::initialize ( const string &_options )
{
    Gvn_Component::initialize ( _options );

    if ( lua_state != _null_ ) {
        lua_close ( lua_state );
    }

    lua_state = luaL_newstate();

    luaopen_base ( lua_state );
    luaopen_coroutine ( lua_state );
    luaopen_table ( lua_state );
    luaopen_io ( lua_state );
    //luaopen_os(lua_state);
    luaopen_string ( lua_state );
    luaopen_bit32 ( lua_state );
    luaopen_math ( lua_state );
    luaopen_debug ( lua_state );
    luaopen_package ( lua_state );

    return 0;
}

int Gvn_Lua::runScriptFile ( const string &_file )
{
    int err = luaL_loadfile ( lua_state, _file.c_str() );
    if ( err != 0 ) {
        return -1;
    }
    lua_pcall ( lua_state, 0, LUA_MULTRET, 0 );
    return 0;
}

int Gvn_Lua::runScriptString ( const string &_script )
{
    int err = luaL_loadstring ( lua_state, _script.c_str() );
    if ( err != 0 ) {
        return -1;
    }
    lua_pcall ( lua_state, 0, LUA_MULTRET, 0 );
    return 0;
}


using namespace std;
void run_lua_test ( int argc = 0, char **argv = _null_ )
{
    string file;
    for ( int n=1; n<argc; ++n ) {
        file += argv[n]; 
    }

    Gvn_Lua my_lua;
    my_lua.initialize();
	
    int err;
    err = my_lua.runScriptFile ( file );
    cout << "Lua Finished with: " << err << " code" << endl << "------------------" << endl;

    my_lua.runScriptString ( "print(\"Hello Lua\")\n"
                             "print(\"\")\n"
                             "print(\"This is a call from gvn_lua\")\n" );
    cout << "Lua Finished with: " << err << " code" << endl << "------------------" << endl;
}

#endif // _GVN_LUA_HEAD_
