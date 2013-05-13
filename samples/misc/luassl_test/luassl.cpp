#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <core/luaviton.hpp>
#include <core/luaviton_ssl.hpp>

using namespace std;


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Run a Lua Test
int main ( int argc , char **argv)
{
    string file;// = "test.lua";

    // GraVitoN::Core::Luaviton        my_lua;
    // GraVitoN::Core::Luaviton_Socket    luasoc(my_lua);
    GraVitoN::Core::Luaviton_SSL &luassl = GraVitoN::Core::Luaviton_SSL::getInstance();

    // luasoc.registerModule();
    luassl.registerModule();

    // luasoc.loadEmAll();
    luassl.loadEmAll();

    cout << " done" << endl;

    if(argc != 2)
    {
        cout<<"Usage: "<<argv[0]<<" [lua script file]"<<endl;
        return 0;
    }

    file = argv[1];

    /// Run script file: test.lua
    cout << " Running script ...";
    GraVitoN::Core::Luaviton::getInstance().runScriptFile(file);
    cout << " done" << endl;

    // my_lua.free(my_lua);

    return 0;
}
