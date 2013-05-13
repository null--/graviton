#ifndef _LDIGEST_H_
#define _LDIGEST_H_

/*
* lmd5.h
* message digest library for Lua 5.1 based on OpenSSL
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 11 Nov 2010 22:58:59
* This code is hereby placed in the public domain.
*/

#include <openssl/opensslv.h>
#include <lua.h>

#undef MYNAME
#undef luaopen_md5
#undef AUTHOR
#undef MD5_CTX
#undef MD5Init
#undef MD5Update
#undef MD5Final
#undef N
#undef Pget
#undef Pnew
#undef Lclone
#undef Ldigest
#undef Lnew
#undef Lreset
#undef Ltostring
#undef Lupdate
#undef R

#define USE_MULTIPLE

// #define USE_MD2_OPENSSL
#define USE_MD4_OPENSSL
#define USE_MD5_OPENSSL
#define USE_SHA1_OPENSSL
#define USE_SHA224_OPENSSL
#define USE_SHA256_OPENSSL
#define USE_SHA384_OPENSSL
#define USE_SHA512_OPENSSL
#define USE_RIPEMD160_OPENSSL
#define USE_MDC2_OPENSSL

LUALIB_API int luaopen_md4(lua_State *L);
LUALIB_API int luaopen_md5(lua_State *L);
LUALIB_API int luaopen_sha1(lua_State *L);
LUALIB_API int luaopen_sha224(lua_State *L);
LUALIB_API int luaopen_sha256(lua_State *L);
LUALIB_API int luaopen_sha384(lua_State *L);
LUALIB_API int luaopen_sha512(lua_State *L);
LUALIB_API int luaopen_ripemd160(lua_State *L);

#endif
