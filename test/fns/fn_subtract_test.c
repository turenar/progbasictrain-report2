#include "config.inc.h"

#include "fns_test_common.h"

static double subtract(double a, double b) {
	return a - b;
}

static void test_fn_subtract_arguments(CuTest* tc) {
	test_common_arguments_bifunc(tc, &mat_fn_subtract);
}

static void test_fn_subtract_value(CuTest* tc) {
	test_common_value_bifunc(tc, &mat_fn_subtract, false, &subtract);
}

CuSuite* test_fn_subtract() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_subtract_arguments);
	SUITE_ADD_TEST(suite, &test_fn_subtract_value);
	return suite;
}
