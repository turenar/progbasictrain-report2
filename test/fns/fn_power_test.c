#include "config.inc.h"

#include <math.h>
#include "fns_test_common.h"

static double power(double a, double b) {
	return pow(a, b);
}

static void test_fn_power_arguments(CuTest* tc) {
	test_common_arguments_bifunc(tc, &mat_fn_power);
}

static void test_fn_power_value(CuTest* tc) {
	test_common_value_bifunc(tc, &mat_fn_power, false, &power);
}

CuSuite* test_fn_power() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_power_arguments);
	SUITE_ADD_TEST(suite, &test_fn_power_value);
	return suite;
}
