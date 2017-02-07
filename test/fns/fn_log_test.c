#include "config.inc.h"

#include <math.h>
#include "fns_test_common.h"
#include "world.h"

static void test_fn_log_arguments(CuTest* tc) {
	test_common_arguments_unifunc(tc, &mat_fn_log);
}

static void test_fn_log_value(CuTest* tc) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, mat_fn_log.name, &mat_fn_log);
#define CHECK_VALUE1(a) test_common_assert_value(tc, world, log(a), "%s[%f]", mat_fn_log.name, (double)a);
	CHECK_VALUE1(1);
	CHECK_VALUE1(2);
	CHECK_VALUE1(2);
//	CHECK_VALUE1(0);
//	CHECK_VALUE1(-1);
	CHECK_VALUE1(0.1);
	CHECK_VALUE1(1.9);
#undef CHECK_VALUE1
	mat_world_free(world);
}

static void test_fn_log_illegal(CuTest* tc) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, mat_fn_log.name, &mat_fn_log);
	CuAssertIntEquals(tc, MAT_ARITHMETIC_ERROR, check_expr_error(tc, world, "Log[0]"));
	CuAssertIntEquals(tc, MAT_ARITHMETIC_ERROR, check_expr_error(tc, world, "Log[-1]"));
	mat_world_free(world);
}

CuSuite* test_fn_log() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_fn_log_arguments);
	SUITE_ADD_TEST(suite, &test_fn_log_value);
	SUITE_ADD_TEST(suite, &test_fn_log_illegal);
	return suite;
}
