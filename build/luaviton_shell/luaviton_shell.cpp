/// LOGGER IS DISABLED
// #undef GVN_ACTIVATE_LOGGER

/// LUA Verbose Mode
#define GVN_VERBOSE_LUA

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
/// @todo Luaviton interactive shell
int main ( int argc , char **argv)
{
    cout << "Luaviton Shell - Part of GraVitoN project" << endl << endl;
	cout << "Loading LUA modules... ";
    cout << "\tcrypto, socket, ssl, zlib, sys, digest" << endl;
    /// Get Instances
    GraVitoN::Core::Luaviton_Crypto &luacrypto  = GraVitoN::Core::Luaviton_Crypto   ::getInstance();
    GraVitoN::Core::Luaviton_Socket &luasoc     = GraVitoN::Core::Luaviton_Socket   ::getInstance();
    GraVitoN::Core::Luaviton_SSL    &luassl     = GraVitoN::Core::Luaviton_SSL      ::getInstance();
    GraVitoN::Core::Luaviton_ZLib   &luazlib    = GraVitoN::Core::Luaviton_ZLib     ::getInstance();
    GraVitoN::Core::Luaviton_Sys    &luasys     = GraVitoN::Core::Luaviton_Sys      ::getInstance();
    GraVitoN::Core::Luaviton_Digest &luadigest  = GraVitoN::Core::Luaviton_Digest   ::getInstance();

    /// REGISTER'EM ALL BEFOR LOAD'EM ALL
    luacrypto   .registerModule();
    luasoc      .registerModule();
    luassl      .registerModule();
    luazlib     .registerModule();
    luasys      .registerModule();
    luadigest   .registerModule();

    /// REGISTER'EM ALL BEFOR LOAD'EM ALL
    luacrypto   .loadEmAll();
    luasoc      .loadEmAll();
    luassl      .loadEmAll();
    luazlib     .loadEmAll();
    luasys      .loadEmAll();
    luadigest   .loadEmAll();

    cout << "[Done]" << endl;

    string file;// = "test.lua";

    if(argc != 2)
    {
        cout<<"Interpreter:"<<endl;
        cout<<"\tUsage: "<<argv[0]<<" [lua script file]"<<endl;
        cout<<""<<endl;
        cout<<"Interactive shell"<<endl;
        cout<<"\tUsage: "<<argv[0]<<" -i"<<endl;
        return 0;
    }

    if ( string(argv[1]) != "-i" )
    {
        file = argv[1];

        /// Run script file: test.lua
        cout << "--- Running script ---" << endl << endl;
        GraVitoN::Core::Luaviton::getInstance().runScriptFile(file);
        cout << endl << "--- [Done] ---" << endl;

        // GraVitoN::Core::Lua::free(my_lua);
    }
    else
    {
        cout << "[EXPERIMENTAL]" << endl;
        cout << "Luaviton Interactive Shell:" << endl;
        cout << "Type 'gameover' for exit" << endl;
        cout << "# ";
        char _cmdline[256];
        string cmdline;
        while(cin.getline(_cmdline, 255))
        {
            cmdline = _cmdline;

            if( cmdline == "gameover" )
                break;
            if( !GraVitoN::Core::Luaviton::getInstance().loadModuleString(cmdline) )
                cout << "<ERROR>" << endl;
            cout << "# ";
        }
    }
    return 0;
}
