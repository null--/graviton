#define GVN_ACTIVATE_LOGGER

#include "graviton.hpp"
#include "core/thread.hpp"
#include <iostream>
using namespace std;


class My_Thread : public GraVitoN::Core::Thread
{
protected:
    virtual bool myMainLoop()
    {
        while(!flag_stop)
        {
            GraVitoN::Core::Logger::logItLn("[My_Thread] I'm Running!");
        }
        GraVitoN::Core::Logger::logItLn("[My_Thread] Main loop is done!");
        return true;
    }
};

int main()
{
    My_Thread thread;

    thread.run();
    cout.flush();

    GraVitoN::Core::Thread::sleep(1000);

    cout<<"[main] Thread supposed to be run!"<<endl;
    thread.stop();

    while (thread.isActive())
        GraVitoN::Core::Thread::sleep(100);

    cout<<"[main] Finished!"<<endl;

    return 0;
}
