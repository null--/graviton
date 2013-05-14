#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <core/luaviton.hpp>
#include <core/luaviton_digest.hpp>

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

    GraVitoN::Core::Luaviton_Digest &luadigest = GraVitoN::Core::Luaviton_Digest::getInstance();

    luadigest.registerModule();

    cout << " done" << endl;

    /// Run script file: test.lua
    cout << " Running script ...";
    GraVitoN::Core::Luaviton::getInstance().runScriptFile(file);
    cout << " done" << endl;

    // GraVitoN::Core::Lua::free(my_lua);

    return 0;
}
