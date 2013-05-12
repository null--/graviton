#ifndef _LUAVITON_CRYPTO_HEAD_
#define _LUAVITON_CRYPTO_HEAD_

#include "graviton.hpp"
#include "core/luaviton.hpp"
#include <external/luacrypto.hpp>

namespace GraVitoN
{

namespace Core
{

// #ifdef GVN_DEBUG
// #endif

class Luaviton_Crypto : public Luaviton_Module
{
public:
    Luaviton_Crypto(Luaviton &_luaviton_instance);

    virtual ~Luaviton_Crypto() {}

    void registerModule()
    {
        luaviton.preloadModule("crypto", luaopen_crypto);
    }

    void loadEmAll()
    {
    }
};

Luaviton_Crypto::Luaviton_Crypto(Luaviton &_luaviton_instance) : Luaviton_Module(_luaviton_instance)
{
}

}
}
#endif
