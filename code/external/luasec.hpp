// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#ifndef _LUASEC_HPP_
#define _LUASEC_HPP_

#include "luasocket.hpp"

extern "C" {
#include "luasocket/socket.h"
#include "luasec/ssl.h"
#include "luasec/context.h"
}

#endif
