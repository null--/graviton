/**
 * @file
 * 
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
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
 * @brief GraVitoN::AI
 * 
 */


#ifndef _GVN_AI_HEAD_
#define _GVN_AI_HEAD_

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <graviton.hpp>

namespace GraVitoN
{
namespace AI
{
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Decision making techniques
 *
 * AI class is master mind of your malware.
 * Whatever desicions you think, your malware want to make during an attack,
 * must be chosen inside this class.
 */
class AI_Component : public Core::Component
{
public:
	/// Constructor
    AI_Component();
	
	/// Destructor
    virtual ~AI_Component();

    /**
	 * @brief Run!
	 *
	 * Decision-making Main-Loop
	 *
	 * @return
	 * true if finished seccessfully
	 */
    virtual bool run() = 0;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
AI_Component::AI_Component()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
AI_Component::~AI_Component()
{
}

}
}
#endif // _GVN_AI_HEAD_
