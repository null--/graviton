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
 * @brief GraVitoN::LUABridge
 *
*/

#ifndef GRAVITON_LUABRIDGE_H
#define GRAVITON_LUABRIDGE_H

#include <graviton.hpp>

#ifdef GVN_ACTIVATE_LUABRIDGE

#include <core/luaviton.hpp>
#include <external/LuaBridge/LuaBridge.h>
#include <external/LuaBridge/RefCountedPtr.h>

namespace GraVitoN
{
/// @todo LUABridge alloc problems
    namespace LUABridge
    {
//    void addClass_Component()
//    {
//        luabridge::getGlobalNamespace ( GraVitoN::Core::Luaviton::getInstance().getState() )
//            .beginNamespace("GraVitoN")
//            .beginNamespace("Core")
//            .beginClass <Core::Component> ("Component")
//            .addConstructor < void(*) (), RefCountedPtr<Core::Component> > ()
//            .endClass()
//            .endNamespace()
//            .endNamespace()
//            ;
//    }
    } /// end of LUABridge
} /// end of GraVitoN

#endif /// end of GVN_ACTIVATE_LUABRIDG
#endif /// end of GRAVITON_LUABRIDGE_H
