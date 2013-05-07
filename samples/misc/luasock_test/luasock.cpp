#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <core/luaviton.hpp>
#include <external/luasocket.hpp>

using namespace std;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Run a Lua Test
int main ( int argc , char **argv)
{
    string file;// = "test.lua";

	if(argc != 2)
	{
		cout<<"Usage: "<<argv[0]<<" [lua script file]"<<endl;
        return 0;
	}

	file = argv[1];

    GraVitoN::Core::Lua::LuaObj my_lua;

    cout << "Initializing ..." << my_lua.lua_state << " - ";
    GraVitoN::Core::Lua::initialize(my_lua);

    // lua_getfield(my_lua.lua_state, LUA_GLOBALSINDEX, "package");
    // lua_getfield(my_lua.lua_state, -1, "preload");
    // lua_pushcfunction(my_lua.lua_state, luaopen_socket_core);
    // lua_setfield(my_lua.lua_state, -2, "socket.core");
    // luaopen_mime_core(my_lua.lua_state);
    // luaopen_socket_core(my_lua.lua_state);

    GraVitoN::Core::Lua::preloadModule(my_lua, "socket.core", luaopen_socket_core);
    GraVitoN::Core::Lua::preloadModule(my_lua, "mime", luaopen_mime_core);
    GraVitoN::Core::Lua::loadModuleFile (my_lua, "socket.lua");
    
    cout << my_lua.lua_state << " done" << endl;

    /// Run script file: test.lua
    cout << " Running script ...";
    GraVitoN::Core::Lua::runScriptFile(my_lua, file);
    cout << " done" << endl;

    // GraVitoN::Core::Lua::free(my_lua);

    return 0;
}
