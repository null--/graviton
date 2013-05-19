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
 * @brief GraVitoN Component
 *
*/

#ifndef _GVN_COMPONENT_HEAD_
#define _GVN_COMPONENT_HEAD_

/// TEST
#pragma once

#include <cstdarg>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// GraVitoN Master Nemspace
namespace GraVitoN
{

namespace Core
{
	
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Father of All Components
class Component
{
public:
	/// Constructor
    Component() {}

	/// Destructor
    virtual ~Component() {}

    virtual bool run() = 0;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
template<class _CS_T_> class Component_Singleton// : public Component
{
protected:
    static _CS_T_ instance;
    // virtual void initialize() = 0;

public:
    // virtual bool run() = 0;
    static _CS_T_ &getInstance()
    {
        // if( !instance )
            // instance = new _CS_T_();
        return instance;
    }
};
template<class _CS_T_> _CS_T_ Component_Singleton<_CS_T_>::instance;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
class Component_Custom: public Component
{
public:
    /**
     * @brief initialize
     *
     * Sample code:
     *
     * va_list vl;
     * va_start(vl, 2); /// TWO arguments
     *
     * ip = va_arg(vl, std::string);
     * port = va_arg(vl, unsigned int);
     *
     * va_end(vl);
     *
     */
    virtual bool initialize(...) = 0;
	virtual bool run() = 0;
};

}
}

#endif // _GVN_COMPONENT_HEAD_
