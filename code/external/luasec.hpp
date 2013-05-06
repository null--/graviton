// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#ifndef _LUASEC_HPP_
#define _LUASEC_HPP_

/// @todo fix luasec and luasocket confilict (maybe: luasec/socket.h --> luasocket/socket.h)
extern "C" {
#include "luasec/ssl.h"
#include "luasec/context.h"
#include "luasec/socket.h"
}

#endif
