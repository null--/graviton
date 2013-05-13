#include "ldigest.h"

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

#ifdef USE_RIPEMD160_OPENSSL
#include <openssl/ripemd.h>
#define MYNAME			"ripemd160"
// #define luaopen_md5		luaopen_ripemd160
#define AUTHOR			"OpenSSL " SHLIB_VERSION_NUMBER
#define MD5_CTX			RIPEMD160_CTX
#define MD5Init			RIPEMD160_Init
#define MD5Update		RIPEMD160_Update
#define MD5Final		RIPEMD160_Final
#define N			RIPEMD160_DIGEST_LENGTH
#ifdef USE_MULTIPLE
#define Pget			ripemd160_Pget
#define Pnew			ripemd160_Pnew
#define Lclone			ripemd160_Lclone
#define Ldigest			ripemd160_Ldigest
#define Lnew			ripemd160_Lnew
#define Lreset			ripemd160_Lreset
#define Ltostring		ripemd160_Ltostring
#define Lupdate			ripemd160_Lupdate
#define R			ripemd160_R
#endif
#endif

/*
* lmd5.c
* MD5 library for Lua 5.1 based on Rivest's API
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 28 Feb 2013 21:10:02
* This code is hereby placed in the public domain.
*/

#include <string.h>

#include "lua.h"
#include "lauxlib.h"

#define MYVERSION	MYNAME " library for " LUA_VERSION " / Feb 2013 / "\
			"using " AUTHOR
#define MYTYPE		MYNAME " context"

static MD5_CTX *Pget(lua_State *L, int i)
{
 return (MD5_CTX*)luaL_checkudata(L,i,MYTYPE);
}

static MD5_CTX *Pnew(lua_State *L)
{
 MD5_CTX *c=(MD5_CTX *)lua_newuserdata(L,sizeof(MD5_CTX));
 luaL_getmetatable(L,MYTYPE);
 lua_setmetatable(L,-2);
 return c;
}

static int Lnew(lua_State *L)			/** new() */
{
 MD5_CTX *c=Pnew(L);
 MD5Init(c);
 return 1;
}

static int Lclone(lua_State *L)			/** clone(c) */
{
 MD5_CTX *c=Pget(L,1);
 MD5_CTX *d=Pnew(L);
 *d=*c;
 return 1;
}

static int Lreset(lua_State *L)			/** reset(c) */
{
 MD5_CTX *c=Pget(L,1);
 MD5Init(c);
 lua_settop(L,1);
 return 1;
}

static int Lupdate(lua_State *L)		/** update(c,s,...) */
{
 MD5_CTX *c=Pget(L,1);
 int i,n=lua_gettop(L);
 for (i=2; i<=n; i++)
 {
  size_t l;
  const char *s=luaL_checklstring(L,i,&l);
  MD5Update(c,s,l);
 }
 lua_settop(L,1);
 return 1;
}
static int Ldigest(lua_State *L)		/** digest(c or s,[raw]) */
{
 unsigned char digest[N];
 if (lua_isuserdata(L,1))
 {
  MD5_CTX c=*Pget(L,1);
  MD5Final(digest,&c);
 }
 else
 {
  size_t l;
  const char *s=luaL_checklstring(L,1,&l);
  MD5_CTX c;
  MD5Init(&c);
  MD5Update(&c,s,l);
  MD5Final(digest,&c);
 }
 if (lua_toboolean(L,2))
  lua_pushlstring(L,(char*)digest,sizeof(digest));
 else
 {
  char *digit="0123456789abcdef";
  char hex[2*N],*h;
  int i;
  for (h=hex,i=0; i<N; i++)
  {
   *h++=digit[digest[i] >> 4];
   *h++=digit[digest[i] & 0x0F];
  }
  lua_pushlstring(L,hex,sizeof(hex));
 }
 return 1;
}

static int Ltostring(lua_State *L)		/** __tostring(c) */
{
 MD5_CTX *c=Pget(L,1);
 lua_pushfstring(L,"%s %p",MYTYPE,(void*)c);
 return 1;
}

static const luaL_Reg R[] =
{
	{ "__tostring",	Ltostring},
	{ "clone",	Lclone	},
	{ "digest",	Ldigest	},
	{ "new",	Lnew	},
	{ "reset",	Lreset	},
	{ "update",	Lupdate	},
	{ NULL,		NULL	}
};

LUALIB_API int luaopen_ripemd160(lua_State *L)
{
 luaL_newmetatable(L,MYTYPE);
 lua_setglobal(L,MYNAME);
 luaL_register(L,MYNAME,R);
 lua_pushliteral(L,"version");			/** version */
 lua_pushliteral(L,MYVERSION);
 lua_settable(L,-3);
 lua_pushliteral(L,"__index");
 lua_pushvalue(L,-2);
 lua_settable(L,-3);
 return 1;
}
