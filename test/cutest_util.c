#include "config.inc.h"

#include "cutest_util.h"
#include <stdio.h>

void CuAssertSizeTEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message,
                                 size_t expected, size_t actual) {
	char buf[STRING_MAX];
	if (expected == actual) {
		return;
	}
	sprintf(buf, "expected <%zu> but was <%zu>", expected, actual);
	CuFail_Line(tc, file, line, message, buf);
}
