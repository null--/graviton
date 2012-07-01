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

#ifndef _GVN_PAYLOAD_HEAD_
#define _GVN_PAYLOAD_HEAD_

#include "../graviton.hpp"
#include <string>
#include <cstring>
using namespace std;

class Gvn_Payload : public Gvn_Component
{
public:
    Gvn_Payload();
    virtual ~Gvn_Payload();

    virtual bool call();
};

Gvn_Payload::Gvn_Payload()
{
	Gvn_Component();
	info.about = "<Gvn_Payload>";
}

Gvn_Payload::~Gvn_Payload()
{
}

bool Gvn_Payload::call()
{
    return true;
}


#endif // _GVN_PAYLOAD_HEAD_
