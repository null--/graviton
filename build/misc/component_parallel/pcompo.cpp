#define GVN_ACTIVATE_LOGGER

#include "core/component_parallel.hpp"
#include "core/thread.hpp"
#include "core/logger.hpp"
#include <iostream>
using namespace std;

class Compo1 : public GraVitoN::Core::Component
{
public:
    bool run()
    {
        GraVitoN::Core::Thread::sleep(1000);
        cout << "Compo1 is up" << endl;
        return true;
    }
};

class Compo2 : public GraVitoN::Core::Component
{
public:
    bool run()
    {
        GraVitoN::Core::Thread::sleep(2000);
        cout << "Compo2 is up" << endl;
        return true;
    }
};

class Compo3 : public GraVitoN::Core::Component
{
public:
    bool run()
    {
        GraVitoN::Core::Thread::sleep(3000);
        cout << "Compo3 is up" << endl;
        return true;
    }
};

int main()
{
    GraVitoN::Core::Component_Parallel compo_man;

    compo_man.runThis(new Compo1);
    compo_man.runThis(new Compo2);
    compo_man.runThis(new Compo3);

	while(compo_man.isActive())
        GraVitoN::Core::Thread::sleep(10);

    cout << "DONE" << endl;
	return 0;
}
