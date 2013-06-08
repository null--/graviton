#ifndef _GVN_LUA_BRIDGE_HEAD_
#define _GVN_LUA_BRIDGE_HEAD_

#include <graviton.hpp>
#ifdef GVN_ACTIVATE_LUABRIDGE

#include <core/luaviton.hpp>
#include <external/LuaBridge/LuaBridge.h>
#include <external/LuaBridge/RefCountedPtr.h>

namespace GraVitoN
{
/// @todo LUABridge alloc problems
namespace LUABridge
{

namespace LUABridge
{

//    void addClass_Component()
//    {
//        luabridge::getGlobalNamespace ( GraVitoN::Core::Luaviton::getInstance().getState() )
//            .beginNamespace("GraVitoN")
//            .beginNamespace("Core")
//            .beginClass <Core::Component> ("Component")
//            .addConstructor < void(*) (), RefCountedPtr<Core::Component> > ()
//            .endClass()
//            .endNamespace()
//            .endNamespace()
//            ;
//    }
}
}
}

#endif

#endif
