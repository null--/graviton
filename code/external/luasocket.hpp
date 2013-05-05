// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#ifndef _LUASOCKET_HPP_
#define _LUASOCKET_HPP_

extern "C" {
#include "luasocket/luasocket.h"
#include "luasocket/socket.h"
#include "luasocket/auxiliar.h"
#include "luasocket/tcp.h"
#include "luasocket/udp.h"
#include "luasocket/mime.h"
}

#endif
