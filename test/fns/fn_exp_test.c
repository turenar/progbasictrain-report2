#include "config.inc.h"

#include <math.h>
#include "fns_test_common.h"

static void test_fn_exp_arguments(CuTest* tc) {
	test_common_arguments_unifunc(tc, &mat_fn_exp);
}

static void test_fn_exp_value(CuTest* tc) {
	test_common_value_unifunc(tc, &mat_fn_exp, false, &exp);
}

CuSuite* test_fn_exp() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_exp_arguments);
	SUITE_ADD_TEST(suite, &test_fn_exp_value);
	return suite;
}
