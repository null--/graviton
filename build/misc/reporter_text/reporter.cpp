#define GVN_ACTIVATE_LOGGER

#include <malkit/generic/reporter_text.hpp>
#include <iostream>

int main()
{
    GraVitoN::Malkit::Reporter_Text reporter;
    reporter.initialize("127.0.0.1", 7357, true);

	reporter.report("[REPORT] Hello report-server\n");
	reporter.report("[REPORT] My message\n");
	reporter.report("[REPORT] Bye\n");

	return 0;
}
