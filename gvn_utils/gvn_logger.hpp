/*
+-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
| This file is part of Graviton.                                       |
|                                                                      |
| Graviton is free software: you can redistribute it and/or modify     |
| it under the terms of the GNU General Public License as published by |
| the Free Software Foundation, either version 3 of the License, or    |
| (at your option) any later version.                                  |
|                                                                      |
| Graviton is distributed in the hope that it will be useful,          |
| but WITHOUT ANY WARRANTY; without even the implied warranty of       |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        |
| GNU General Public License for more details.                         |
|                                                                      |
| You should have received a copy of the GNU General Public License    |
| along with Graviton.  If not, see <http://www.gnu.org/licenses/>.    |
+-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+
*/

/*
 * Programmers:
 * 		1- Sina Hatef Matbue | _null_ | sinahatef.cpp@gmail.com
 */

/*
 Attention:
     DO NOT FORGET TO SET LOGGER FLAG, IF YOU WANT TO STORE RUN TIME LOGS:
         gcc -D GVN_ACTIVATE_LOGGER <...>
 */

#ifndef _GVN_LOGGER_HEAD_
#define _GVN_LOGGER_HEAD_

#include "../graviton.hpp"
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

template < class _C_T_ > bool gvnLogIt ( const _C_T_ &log )
{
#ifdef GVN_ACTIVATE_LOGGER
    //freopen(GVN_LOG_FILE, "w+", stdout);
    cout << log;
    return true;
#else
    return true;
#endif
}

bool gvnLogEndl()
{
    gvnLogIt ( "\n" );
    return true;
}

template < class _C_T_ > bool gvnLogItLn ( const _C_T_ &log )
{
    gvnLogIt ( log );
    gvnLogEndl();
    return true;
}

#endif // _GVN_LOGGER_HEAD_

