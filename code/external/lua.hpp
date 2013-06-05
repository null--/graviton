// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#ifndef _LUA_HPP_
#define _LUA_HPP_

#ifdef __cplusplus

extern "C" {
#endif
    #include <external/lua5.1/lua.h>
    #include <external/lua5.1/lauxlib.h>
    #include <external/lua5.1/lualib.h>
#ifdef __cplusplus
}
#endif

#endif
