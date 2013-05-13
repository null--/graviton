#ifndef _LUAVITON_SYS_HEAD_
#define _LUAVITON_SYS_HEAD_

#include "graviton.hpp"
#include "core/luaviton.hpp"
#include <external/luasys.hpp>

namespace GraVitoN
{

namespace Core
{

// #ifdef GVN_DEBUG
// #endif

class Luaviton_Sys : public Luaviton_Module<Luaviton_Sys>
{
public:
    Luaviton_Sys ();//(Luaviton &_luaviton_instance);

    virtual ~Luaviton_Sys() {}

    void registerModule()
    {
        luaviton.preloadModule("sys", luaopen_sys);
        luaviton.preloadModule("sys.sock", luaopen_sys_sock);
    }

    void loadEmAll()
    {
    }
};

Luaviton_Sys::Luaviton_Sys ()//(Luaviton &_luaviton_instance) : Luaviton_Module(_luaviton_instance)
{
}

}
}
#endif
