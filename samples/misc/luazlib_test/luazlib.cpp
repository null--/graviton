#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <core/luaviton.hpp>
#include <external/luazlib.hpp>

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

    my_lua.preloadModule("zlib", luaopen_zlib);
    my_lua.loadModuleFile ("gzip.lua");

    cout << " done" << endl;

    /// Run script file: test.lua
    cout << " Running script ...";
    my_lua.runScriptFile(file);
    cout << " done" << endl;

    // my_lua.free(my_lua);

    return 0;
}
