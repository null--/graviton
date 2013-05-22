#define GVN_ACTIVATE_LOGGER

#include <iostream>
#include <graviton.hpp>
#include <core/luaviton.hpp>
#include <core/luabridge.hpp>
using namespace std;

// using namespace luabridge;

//==============================================================================

/*
 * Test classes
 */

bool g_success = true;

class A
{
protected:
    string name;
    mutable bool success;

public:
    static int testStaticProp;

    A (string const& name_) : name (name_), success (false), testProp (22)
    {
        cout << "[C] A:Constructor" << endl;
    }
    virtual ~A ()
    {
         cout << "[C] A:Destructor" << endl;
    }

    void testSucceeded() const
    {
        cout << "[C] A:Cool!" << endl;
    }

    virtual void testVirtual ()
    {
        cout << "[C] A:testVirtual" << endl;
    }

    const char * getName () const
    {
        return name.c_str();
    }

    static void testStatic ()
    {
        cout << "[C] A:testStatic" << endl;
    }

    int testProp;

    int testPropGet () const
    {
        cout << "[C] A:Get Value" << endl;
        return testProp;
    }
    void testPropSet (int x)
    {
        testProp = x;
        cout << "[C] A:Set Value" << endl;
    }
};

class B: public A
{
public:
    B (string const& name_) : A (name_)
    {
        cout << "[C] B:Constructor" << endl;
    }
    virtual ~B()
    {
        cout << "[C] A:Desstructor" << endl;
    }
    virtual void testVirtual()
    {
        cout << "[C] B:testVirtual" << endl;
    }
};

class C
{
public:
    C(int)
    {

    }
};

int A::testStaticProp = 47;

/*
 * Test functions
 */

void testData (A * a)
{
    a->testPropSet(99);
}

string testRetStdString ()
{
    static string ret ("Hello, world");
    return ret;
}

void testParamStdStringRef (const string &str)
{
    cout << "[C] " << str << endl;
}

RefCountedPtr <A> testRetSharedPtrA ()
{
  static RefCountedPtr <A> sp_A (new A("from C"));
  return sp_A;
}

RefCountedPtr <A const> testRetSharedPtrConstA ()
{
  static RefCountedPtr <A> sp_A (new A("const A"));
  return sp_A;
}

void function()
{
    cout << "HELL YEAH!" << endl;
}

void callback_func( void *func )
{
    cout << func << endl;
    void (*func_p) (void *);
    func_p = (void (*) (void*))func;
    func_p(NULL);
}

// add our own functions and classes to a Lua environment
void testLuaBridge (lua_State *L)
{
    luabridge::getGlobalNamespace (L)
        .addFunction ("testRetStdString", &testRetStdString)
        .addFunction ("testParamStdStringRef", &testParamStdStringRef)
        .addFunction ("testData", &testData)
        .beginClass <A> ("A")
            .addConstructor <void (*) (const string &), RefCountedPtr <A> > ()
            .addFunction ("testVirtual", &A::testVirtual)
            .addFunction ("getName", &A::getName)
            .addFunction ("testSucceeded", &A::testSucceeded)
            .addData ("testProp", &A::testProp)
            .addFunction ("testPropSet", &A::testPropSet)
            .addFunction ("testPropGet", &A::testPropGet)
            .addStaticFunction ("testStatic", &A::testStatic)
            .addStaticData ("testStaticProp", &A::testStaticProp)
        .endClass ()
        .deriveClass <B, A> ("B")
            .addConstructor <void (*) (const string &), RefCountedPtr <B> > ()
            .addFunction ("testVirtual", &B::testVirtual)
            .addStaticFunction ("testStatic", &B::testStatic)
        .endClass ()
        .beginClass <C> ("C")
            .addConstructor <void (*) (int), RefCountedPtr <C> > ()
        .endClass()
        .addFunction ("testRetSharedPtrA", &testRetSharedPtrA)
        .addFunction ("testRetSharedPtrConstA", &testRetSharedPtrConstA)
        ;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Test Lua Functionality
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
    callback_func((void*)&function);

    string file;// = "test.lua";

    if(argc != 2)
    {
        cout<<"Usage: "<<argv[0]<<" [lua script file]"<<endl;
        return 0;
    }

    file = argv[1];

    GraVitoN::Core::Luaviton &my_lua = GraVitoN::Core::Luaviton::getInstance();

    /// Register Function
    cout << " Registering function ...";
    my_lua.regiserFunction ( "testFunc", &testFunc );
    cout << " done" << endl;

    testLuaBridge(my_lua.getState());
    // my_lua.free(my_lua);

    /// Run script file: test.lua
    cout << " Running script ...";
    my_lua.runScriptFile( file);
    cout << " done" << endl;

    /// Run script string
    my_lua.runScriptString (
                "a = testFunc(\"Hello GraVitoN\", 3)\n"
                "print(\"runScriptString: a= \", a)\n"
                "print(\"runScriptString: Done\")\n"
                );
    cout << "global_data= " << global_data << endl;

    return 0;
}
