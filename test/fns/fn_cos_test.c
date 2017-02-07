#include "config.inc.h"

#include <math.h>
#include "fns_test_common.h"

static void test_fn_cos_arguments(CuTest* tc) {
	test_common_arguments_unifunc(tc, &mat_fn_cos);
}

static void test_fn_cos_value(CuTest* tc) {
	test_common_value_unifunc(tc, &mat_fn_cos, false, &cos);
}

CuSuite* test_fn_cos() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_cos_arguments);
	SUITE_ADD_TEST(suite, &test_fn_cos_value);
	return suite;
}
