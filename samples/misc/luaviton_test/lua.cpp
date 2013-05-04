#define GVN_ACTIVATE_LOGGER

#include <iostream>
#include <graviton.hpp>
#include <core/luaviton.hpp>
using namespace std;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Test Lua Functionality
/// DO NOT FORGET TO COPY test.lua to your build directory

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

string global_data = "UNCHANGED";

/// A registered function
int testFunc ( lua_State *stt )
{
    /// Parse Arguments
    int argc = lua_gettop ( stt );
	cout << "testFunc: Num of Args= " << argc <<endl;
    string text = lua_tostring ( stt, 1 ); //<First Argument
    int num = lua_tointeger ( stt, 2 ); // Second Argument

    cout << "testFunc: num= " << num << endl;
    /// Do what you supposed to do
    for ( int i=0; i<num; ++i ) {
        cout << "testFunc: Hell'o GraVitoN!" << endl;
    }

    /// Change global_data
    global_data = "CHANGED";

    /// Push Return Result
    lua_pushnumber ( stt, 7357 );

    return 1; ///< number of returned values
}

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

	GraVitoN::Lua my_lua;
    my_lua.initialize();

    /// Register Function
    my_lua.regiserFunction ( "testFunc", &testFunc );

	bool err;

    /// Run script file: test.lua
	err = my_lua.runScriptFile(file);

	/// Test re-initialization
	my_lua.initialize();

    /// Run script string
    err = my_lua.runScriptString (
					"a = testFunc(\"Hello GraVitoN\", 3)\n"
				    "print(\"runScriptString: a= \", a)\n"
				    "print(\"runScriptStrin: Done\")\n"
				 );
	cout << "global_data= " << global_data << endl;

    return 0;
}
