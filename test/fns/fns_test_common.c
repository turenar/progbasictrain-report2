#include "config.inc.h"

#include "fns_test_common.h"
#include "world.h"
#include <stdio.h>
#include <expr.h>
#include "parser.h"

/*
 * parse test
 *  * arguments missing
 *  * arguments too many
 *  * value check
 */

static void assert_parse_failure(CuTest*, mat_world_t*, const char*, ...) __attribute_format__(printf, 3, 4);
static void assert_value(CuTest*, mat_world_t*, double, const char*, ...) __attribute_format__(printf, 4, 5);

void test_common_arguments_bifunc(CuTest* tc, mat_op_def_t* op_def) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, op_def->name, op_def);
	assert_parse_failure(tc, world, "%s[]", op_def->name);
	assert_parse_failure(tc, world, "%s[1]", op_def->name);
	assert_parse_failure(tc, world, "%s[1,2,3]", op_def->name);
	mat_world_free(world);
}

void test_common_value_bifunc(CuTest* tc, mat_op_def_t* op_def, bool zero_denied, double(* fn)(double, double)) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, op_def->name, op_def);
#define CHECK_VALUE(a, b) assert_value(tc, world, fn(a, b), "%s[%f,%f]", op_def->name, (double)a, (double)b);
	CHECK_VALUE(1, 1);
	CHECK_VALUE(1, 2);
	CHECK_VALUE(2, 1);
	CHECK_VALUE(2, 2);
	if (!zero_denied) {
		CHECK_VALUE(0, 0);
	}
	CHECK_VALUE(-1, -1);
	CHECK_VALUE(0.1, 0.5);
	CHECK_VALUE(1.9, 4.9);
#undef CHECK_VALUE
	mat_world_free(world);
}

static void assert_parse_failure(CuTest* tc, mat_world_t* w, const char* fmt, ...) {
	va_list ap;
	char buf[65536];
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	mat_parser_t* parser = mat_parser_new(w, buf);
	CuAssertPtrEquals(tc, NULL, mat_parser_parse(parser));
	mat_parser_free(parser);
}

static void assert_value(CuTest* tc, mat_world_t* w, double ex, const char* fmt, ...) {
	va_list ap;
	char buf[65536];
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	mat_parser_t* parser = mat_parser_new(w, buf);
	mat_expr_t* expr = mat_parser_parse(parser);
	CuAssertPtrNotNullMsg(tc, buf, expr);
	mpq_t t = {0};
	mpq_init(t);
	mat_error_t err = expr->op_def->calc_value(expr, t);
	CuAssertIntEquals_Msg(tc, buf, MAT_SUCCESS, err);
	CuAssertDblEquals_Msg(tc, buf, ex, mpq_get_d(t), 0.000001);
	mpq_clear(t);
	mat_expr_free(expr);
	mat_parser_free(parser);
}
