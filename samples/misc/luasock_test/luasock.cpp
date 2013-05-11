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

    GraVitoN::Core::Luaviton my_lua;

    // lua_getfield(my_lua.lua_state, LUA_GLOBALSINDEX, "package");
    // lua_getfield(my_lua.lua_state, -1, "preload");
    // lua_pushcfunction(my_lua.lua_state, luaopen_socket_core);
    // lua_setfield(my_lua.lua_state, -2, "socket.core");
    // luaopen_mime_core(my_lua.lua_state);
    // luaopen_socket_core(my_lua.lua_state);

    my_lua.preloadModule("socket.core", luaopen_socket_core);
    my_lua.preloadModule("mime", luaopen_mime_core);
    my_lua.loadModuleFile ("socket.lua");
    
    cout <<  " done" << endl;

    /// Run script file: test.lua
    cout << " Running script ...";
    my_lua.runScriptFile(file);
    cout << " done" << endl;

    // my_lua.free(my_lua);

    return 0;
}
