#include "config.inc.h"

#include "fns_test_common.h"

static double divide(double a, double b) {
	return a / b;
}

static void test_fn_divide_arguments(CuTest* tc) {
	test_common_arguments_bifunc(tc, &mat_fn_divide);
}

static void test_fn_divide_value(CuTest* tc) {
	test_common_value_bifunc(tc, &mat_fn_divide, true, &divide);
}

CuSuite* test_fn_divide() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_divide_arguments);
	SUITE_ADD_TEST(suite, &test_fn_divide_value);
	return suite;
}
