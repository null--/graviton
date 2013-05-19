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
 * @brief GraVitoN::Malkit
 *
*/

#ifndef _GVN_MALKIT_HEAD_
#define _GVN_MALKIT_HEAD_

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
#include <graviton.hpp>
#include <core/logger.hpp>

namespace GraVitoN
{

namespace Malkit
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief MALware toolKIT
 *
 * All your malicious weapons (modifying registery, adding GraVitoN to startup list,
 * privilage escalation, etc), are a child of this class.
 *
 */
class Malkit_Component : public Core::Component
{
public:
	/// Constructor
    Malkit_Component();

	/// Destructor
    virtual ~Malkit_Component();

	/**
	 * @brief Use your malicious weapon
	 * 
	 * @return
	 * true if job is done!
	 * 
	 */
    virtual bool run() = 0;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Malkit_Component::Malkit_Component()
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
Malkit_Component::~Malkit_Component()
{

}

}
}
#endif //_GVN_MALKIT_HEAD_
