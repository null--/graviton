#ifndef _GVN_LUA_BRIDGE_HEAD_
#define _GVN_LUA_BRIDGE_HEAD_

#include <graviton.hpp>
#ifdef GVN_ACTIVATE_LUABRIDGE

#include <core/luaviton.hpp>
#include <external/LuaBridge/LuaBridge.h>
#include <external/LuaBridge/RefCountedPtr.h>

// #include <core/thread.hpp>
namespace GraVitoN
{
namespace Core
{
namespace LUABridge
{

void addClass_Component()
{
    luabridge::getGlobalNamespace ( Core::Luaviton::getInstance().getState() )
            .beginNamespace("gvn")
            .beginNamespace("core")

            .endNamespace()
            .endNamespace()
    ;
}
    
}
}
}

#endif

#endif
