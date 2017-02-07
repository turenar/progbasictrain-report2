#include "config.inc.h"

#include <math.h>
#include "fns_test_common.h"

static void test_fn_sin_arguments(CuTest* tc) {
	test_common_arguments_unifunc(tc, &mat_fn_sin);
}

static void test_fn_sin_value(CuTest* tc) {
	test_common_value_unifunc(tc, &mat_fn_sin, false, &sin);
}

CuSuite* test_fn_sin() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_sin_arguments);
	SUITE_ADD_TEST(suite, &test_fn_sin_value);
	return suite;
}
