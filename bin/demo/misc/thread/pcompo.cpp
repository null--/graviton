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
 * @brief test parallel components
 *
 */

#define GVN_ACTIVATE_LOGGER

#include <utils/component_parallel.hpp>
#include <core/thread.hpp>
#include <core/logger.hpp>
#include <iostream>
using namespace std;

class Compo1 : public GraVitoN::Core::Component
{
    string name;

public:
    Compo1(const string &_name)
    {
        name = _name;
    }

    bool run()
    {
        GraVitoN::Core::sleep(1000);
        cout << name << " is up" << endl;
        return true;
    }
};

class Compo2 : public GraVitoN::Core::Component
{
public:
    bool run()
    {
        GraVitoN::Core::sleep(2000);
        cout << "Compo2 is up" << endl;
        return true;
    }
};

class Compo3 : public GraVitoN::Core::Component
{
public:
    bool run()
    {
        GraVitoN::Core::sleep(3000);
        cout << "Compo3 is up" << endl;
        return true;
    }
};

int main()
{
    GraVitoN::Utils::Component_Parallel compo_man;

    Compo1 C11("C11"), C12("C12");
    Compo2 C2;
    Compo3 C3;
    compo_man.runThis(C11);
    compo_man.runThis(C12);
    compo_man.runThis(C2);
    compo_man.runThis(C3);

	while( compo_man.isActive() )
        GraVitoN::Core::sleep(10);

    cout << "DONE" << endl;
	return 0;
}
