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
 * @brief GraVitoN::Infect::Infect_Component
 *
*/

#ifndef _GVN_SPREAD_HEAD_
#define _GVN_SPREAD_HEAD_
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <graviton.hpp>

namespace GraVitoN
{

namespace Infect
{
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Infection techniques
 *
 * The main idea of Spreading is to design components that letting us pwn
 * new victims during an attack.\n
 * examples:\n
 * 1. a windows virus spreads using PE file infection \n
 * 2. a windows worm spreads using ms08_67 exploit \n
 */
class Infect_Component : public Core::Component
{
public:
	/// Constructor
    Infect_Component();
	
	/// Destructor
    virtual ~Infect_Component();

	/**
     * @brief Execute Spread Technique
	 *
	 * Calling this method means you want to DO someone/something!
	 * 
	 * @return
	 * Was it successfull, or not?!
	 * 
	 */ 
    virtual bool run() = 0;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Infect_Component::Infect_Component()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Infect_Component::~Infect_Component()
{
}


}
}

#endif // _GVN_SPREAD_HEAD_
