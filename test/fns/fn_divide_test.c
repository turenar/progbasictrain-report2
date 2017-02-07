#include "config.inc.h"

#include "expr.h"
#include "fns_test_common.h"
#include "world.h"

static double divide(double a, double b) {
	return a / b;
}

static void test_fn_divide_arguments(CuTest* tc) {
	test_common_arguments_bifunc(tc, &mat_fn_divide);
}

static void test_fn_divide_value(CuTest* tc) {
	test_common_value_bifunc(tc, &mat_fn_divide, true, &divide);
}

static void test_fn_divide_by_zero(CuTest* tc) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, mat_fn_divide.name, &mat_fn_divide);
	CuAssertIntEquals(tc, MAT_DIVIDED_BY_ZERO, check_expr_error(tc, world, "Divide[0,0]"));
	mat_world_free(world);
}

CuSuite* test_fn_divide() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_divide_arguments);
	SUITE_ADD_TEST(suite, &test_fn_divide_value);
	SUITE_ADD_TEST(suite, &test_fn_divide_by_zero);
	return suite;
}
