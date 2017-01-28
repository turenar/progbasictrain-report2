#include "config.inc.h"

#include "fns_test_common.h"

static double times(double a, double b) {
	return a * b;
}

static void test_fn_times_arguments(CuTest* tc) {
	test_common_arguments_bifunc(tc, &mat_fn_times);
}

static void test_fn_times_value(CuTest* tc) {
	test_common_value_bifunc(tc, &mat_fn_times, false, &times);
}

CuSuite* test_fn_times() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_times_arguments);
	SUITE_ADD_TEST(suite, &test_fn_times_value);
	return suite;
}
