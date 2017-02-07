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
	CuSuiteAddSuite(suite, test_fn_cos());
	CuSuiteAddSuite(suite, test_fn_sin());
	CuSuiteAddSuite(suite, test_fn_power());
	CuSuiteAddSuite(suite, test_fn_exp());
	CuSuiteAddSuite(suite, test_fn_log());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	fflush(stdout);
	return suite->failCount != 0;
}
