#define GVN_ACTIVATE_LOGGER

#include "../../../gvn_malkit/generic/reporter/gvn_malkit_reporter_text.hpp"
#include <iostream>

int main()
{
	Malkit_Reporter_Text reporter;
	reporter.initialize("IP=127.0.0.1 PORT=7357 STAY_CONNECTED=true");

	reporter.report("[REPORT] Hello report-server\n");
	reporter.report("[REPORT] My message\n");
	reporter.report("[REPORT] Bye\n");

	return 0;
}
