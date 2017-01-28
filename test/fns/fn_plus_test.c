#include "config.inc.h"

#include "fns_test_common.h"

static double plus(double a, double b) {
	return a + b;
}

static void test_fn_plus_arguments(CuTest* tc) {
	test_common_arguments_bifunc(tc, &mat_fn_plus);
}

static void test_fn_plus_value(CuTest* tc) {
	test_common_value_bifunc(tc, &mat_fn_plus, false, &plus);
}

CuSuite* test_fn_plus() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_plus_arguments);
	SUITE_ADD_TEST(suite, &test_fn_plus_value);
	return suite;
}
