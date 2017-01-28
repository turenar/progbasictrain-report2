#include "config.inc.h"

#include "fns_test.h"
#include <stdio.h>
#include <cutest/CuTest.h>

int main() {
	CuString* output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, test_fn_plus());
	CuSuiteAddSuite(suite, test_fn_subtract());
	CuSuiteAddSuite(suite, test_fn_times());
	CuSuiteAddSuite(suite, test_fn_divide());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	fflush(stdout);
	return suite->failCount != 0;
}
