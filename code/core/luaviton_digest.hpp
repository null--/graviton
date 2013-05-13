#ifndef _Luaviton_Digest_HEAD_
#define _Luaviton_Digest_HEAD_

#include "graviton.hpp"
#include "core/luaviton.hpp"
#include <external/luadigest.hpp>

namespace GraVitoN
{

namespace Core
{

// #ifdef GVN_DEBUG
// #endif

/// Digests: md2, md4, md5, sha1, sha224, sha256, sha384, sha512, ripemd160, mdc2.
class Luaviton_Digest : public Luaviton_Module<Luaviton_Digest>
{
public:
    Luaviton_Digest (); // (Luaviton &_luaviton_instance);

    virtual ~Luaviton_Digest() {}

    void registerModule()
    {
        luaviton.preloadModule("md4", luaopen_md4);
        luaviton.preloadModule("md5", luaopen_md5);
        luaviton.preloadModule("sha1", luaopen_sha1);
        luaviton.preloadModule("ripemd160", luaopen_ripemd160);
        luaviton.preloadModule("sha224", luaopen_sha224);
        luaviton.preloadModule("sha256", luaopen_sha256);
        luaviton.preloadModule("sha384", luaopen_sha384);
        luaviton.preloadModule("sha512", luaopen_sha512);
//        luaopen_md4(luaviton.getState());
//        luaopen_md5(luaviton.getState());
//        luaopen_sha1(luaviton.getState());
//        luaopen_ripemd160(luaviton.getState());
//        luaopen_sha224(luaviton.getState());
//        luaopen_sha256(luaviton.getState());
//        luaopen_sha384(luaviton.getState());
//        luaopen_sha512(luaviton.getState());
    }

    bool loadEmAll()
    {
        return true;
    }
};

Luaviton_Digest::Luaviton_Digest ()
{
}

}
}
#endif
