#define GVN_ACTIVATE_LOGGER

#include "../../../gvn_payload/linux/gvn_payload_linux64b.hpp"
#include "../../../gvn_payload/linux/gvn_payload_linux64r.hpp"
#include "../../../gvn_component_parallel.hpp"
#include "../../../gvn_utils/gvn_thread.hpp"
#include "../../../gvn_utils/gvn_logger.hpp"
#include <iostream>
using namespace std;

int main()
{
	Payload_Linux64b pay_lb, pay_lb2;
	Payload_Linux64r pay_lr;

	GraVitoN::Component_Parallel compo_man;
	compo_man.runThis(&pay_lb, "PORT=7356");
	compo_man.runThis(&pay_lb2, "PORT=7357");
	compo_man.runThis(&pay_lr, "PORT=7358 HOST='192.168.22.100'");

	while(compo_man.isActive())
		GraVitoN::Thread::sleep(10);

	return 0;
}
