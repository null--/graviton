/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief GraVitoN::Socket
 *
*/

#ifndef _GVN_SOCKET_HEAD_
#define _GVN_SOCKET_HEAD_

#include <graviton.hpp>
#include <external/ting/net/Lib.hpp>

namespace GraVitoN
{

namespace Core
{

class Socket
{
    private:
        ting::net::Lib *socket_lib;

    public:
        Socket();
        ~Socket();
};

Socket::Socket()
{
    if( !ting::net::Lib::IsCreated() )
        socket_lib = new ting::net::Lib();
    else
        socket_lib = &(ting::net::Lib::Inst());
}

Socket::~Socket()
{

}

}
}

#endif // _GVN_SOCKET_HEAD_
