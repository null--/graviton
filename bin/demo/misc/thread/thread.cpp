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
 * @brief test thread class
 *
 */

#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/thread.hpp>
#include <core/memory.hpp>
#include <iostream>
using namespace std;


class My_Thread : public GraVitoN::Core::Thread
{
private:
    GraVitoN::Core::Memory<char> buf;
    
protected:
    virtual bool main()
    {
        if( !buf.alloc(256) )
            return false;
        
        
        printf("Buffer Address: %x\n", buf.address());

        char i = 0;
        while( 1 )
        {
            ++i;
            buf.set(i);
            cout << (int) *buf.address() << endl;
            GraVitoN::Core::sleep(1);
        }
        GraVitoN::Core::Logger::logItLn("[My_Thread] Main loop is done!");

        return true;
    }
    
public:
    ~My_Thread()
        {
            buf.free();
        }
};

int main()
{
    My_Thread thread;

    thread.run();
    cout.flush();

    GraVitoN::Core::sleep(3000);

    cout<<"[main] Thread supposed to be run!"<<endl;
    // thread.wait();

    thread.stop();
    
    while (thread.isActive())
    {
         GraVitoN::Core::sleep(100);
    }

    GraVitoN::Core::sleep(1000);
    
    cout<<"[main] Finished!"<<endl;

    return 0;
}
