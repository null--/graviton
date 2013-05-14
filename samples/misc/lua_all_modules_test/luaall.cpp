/// LOGGER IS DISABLED
#undef GVN_ACTIVATE_LOGGER
// #define GVN_ACTIVATE_LOGGER

/// DEBUG MODE IS DISABLED
#undef GVN_DEBUG

#include <graviton.hpp>
#include <iostream>
#include <core/luaviton.hpp>

#include <core/luaviton_crypto.hpp>
#include <core/luaviton_socket.hpp>
#include <core/luaviton_ssl.hpp>
#include <core/luaviton_zlib.hpp>
#include <core/luaviton_sys.hpp>
#include <core/luaviton_digest.hpp>
using namespace std;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Run a Complete Lua Test
int main ( int argc , char **argv)
{
	cout << "Loading LUA modules... ";
    /// Get Instances
    GraVitoN::Core::Luaviton_Crypto &luacrypto  = GraVitoN::Core::Luaviton_Crypto   ::getInstance();
    GraVitoN::Core::Luaviton_Socket &luasoc     = GraVitoN::Core::Luaviton_Socket   ::getInstance();
    GraVitoN::Core::Luaviton_SSL    &luassl     = GraVitoN::Core::Luaviton_SSL      ::getInstance();
    GraVitoN::Core::Luaviton_ZLib   &luazlib    = GraVitoN::Core::Luaviton_ZLib     ::getInstance();
    GraVitoN::Core::Luaviton_Sys    &luasys     = GraVitoN::Core::Luaviton_Sys      ::getInstance();
    GraVitoN::Core::Luaviton_Digest &luadigist  = GraVitoN::Core::Luaviton_Digest   ::getInstance();

    /// REGISTER'EM ALL BEFOR LOAD'EM ALL
    luacrypto   .registerModule();
    luasoc      .registerModule();
    luassl      .registerModule();
    luazlib     .registerModule();
    luasys      .registerModule();
    luadigist   .registerModule();

    /// REGISTER'EM ALL BEFOR LOAD'EM ALL
    luacrypto   .loadEmAll();
    luasoc      .loadEmAll();
    luassl      .loadEmAll();
    luazlib     .loadEmAll();
    luasys      .loadEmAll();
    luadigist   .loadEmAll();

    cout << "[Done]" << endl;

    string file;// = "test.lua";

    if(argc != 2)
    {
        cout<<"Usage: "<<argv[0]<<" [lua script file]"<<endl;
        return 0;
    }

    file = argv[1];

    /// Run script file: test.lua
    cout << " Running script... " << endl;
    GraVitoN::Core::Luaviton::getInstance().runScriptFile(file);
    cout << "--- [Done] ---" << endl;

    // GraVitoN::Core::Lua::free(my_lua);

    return 0;
}
