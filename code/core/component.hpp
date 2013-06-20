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

#ifndef GRAVITON_COMPONENT_H
#define GRAVITON_COMPONENT_H

// #pragma once
#include <graviton.hpp>
#include <cstdarg>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// GraVitoN Nemspace
namespace GraVitoN
{
/// GraVitoN Core
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

            /// @todo isActive to be or not to be
            // virtual bool isActive() = 0;
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class _CS_T_> class Component_Singleton// : public Component
        {
        protected:
            /// Instance of singleton class
            static _CS_T_ instance;
            // virtual void initialize() = 0;

        public:
            // virtual bool run() = 0;

            /// Get singleton instance
            static _CS_T_ &getInstance();
        };
        template<class _CS_T_> _CS_T_ Component_Singleton<_CS_T_>::instance;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        static _CS_T_ & Component_Singleton::getInstance()
        {
            // if( !instance )
            // instance = new _CS_T_();
            return instance;
        }

    } // Core
} // GraVitoN

#endif // GRAVITON_COMPONENT_H
