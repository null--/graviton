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

class Luaviton_Crypto : public Luaviton_Module<Luaviton_Crypto>
{
public:
    Luaviton_Crypto();

    virtual ~Luaviton_Crypto() {}

    void registerModule()
    {
        luaviton.preloadModule("crypto", luaopen_crypto);
    }

    void loadEmAll()
    {
    }
};

Luaviton_Crypto::Luaviton_Crypto()
{
}

}
}
#endif
