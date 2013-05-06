// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#ifndef _LUA_HPP_
#define _LUA_HPP_

#ifdef __cplusplus
extern "C" {
#endif
    #include "lua5.1/lua.h"
    #include "lua5.1/lualib.h"
    #include "lua5.1/lauxlib.h"
#ifdef __cplusplus
}
#endif

#endif
