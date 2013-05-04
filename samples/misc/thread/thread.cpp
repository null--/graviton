#define GVN_ACTIVATE_LOGGER

#include "../../../graviton.hpp"
#include "../../../gvn_utils/gvn_thread.hpp"
#include "../../../gvn_utils/gvn_logger.hpp"
#include <iostream>
using namespace std;

class My_Thread : public GraVitoN::Thread
{
protected:
	virtual bool myMainLoop()
	{
		while(!flag_stop)
		{
			GraVitoN::Logger::logItLn("[My_Thread] I'm Running!");
		}
		GraVitoN::Logger::logItLn("[My_Thread] Main loop is done!");
		return true;
	}
};

int main()
{
	My_Thread thread;
	thread.initialize("");
	thread.run();
	cout.flush();

	gvnSleep(10);

	cout<<"[main] Thread supposed to be run!"<<endl;
	thread.stop();

	cout<<"[main] Finished!"<<endl;

	return 0;
}
