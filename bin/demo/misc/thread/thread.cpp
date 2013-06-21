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
