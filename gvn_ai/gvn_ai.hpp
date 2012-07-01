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

#ifndef _GVN_AI_HEAD_
#define _GVN_AI_HEAD_

#include "../graviton.hpp"
#include "../gvn_payload/gvn_payload.hpp"
#include "../gvn_concealer/gvn_concealer.hpp"
#include "../gvn_intercross/gvn_intercross.hpp"

class Gvn_AI : public Gvn_Component
{
public:
    Gvn_AI();
    virtual ~Gvn_AI();

	virtual bool think();
};

Gvn_AI::Gvn_AI()
{
	Gvn_Component();
	info.about = "<Gvn_AI>";
}

Gvn_AI::~Gvn_AI()
{
}

bool Gvn_AI::think()
{
	gvnLogItLn("thinking...");
	
	return 0;
}

#endif // _GVN_AI_HEAD_
