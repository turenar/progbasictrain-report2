#include "config.inc.h"

#include "fns_test_common.h"
#include <stdio.h>
#include "expr.h"
#include "parser.h"
#include "world.h"

/*
 * parse test
 *  * arguments missing
 *  * arguments too many
 *  * value check
 */


void test_common_arguments_bifunc(CuTest* tc, mat_op_def_t* op_def) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, op_def->name, op_def);
	test_common_assert_parse_failure(tc, world, "%s[]", op_def->name);
	test_common_assert_parse_failure(tc, world, "%s[1]", op_def->name);
	test_common_assert_parse_failure(tc, world, "%s[1,2,3]", op_def->name);
	mat_world_free(world);
}

void test_common_arguments_unifunc(CuTest* tc, mat_op_def_t* op_def) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, op_def->name, op_def);
	test_common_assert_parse_failure(tc, world, "%s[]", op_def->name);
	test_common_assert_parse_failure(tc, world, "%s[1,2]", op_def->name);
	mat_world_free(world);
}

void test_common_value_bifunc(CuTest* tc, mat_op_def_t* op_def, bool zero_denied, double(* fn)(double, double)) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, op_def->name, op_def);
#define CHECK_VALUE2(a, b) test_common_assert_value(tc, world, fn(a, b), "%s[%f,%f]", op_def->name, (double)a, (double)b);
	CHECK_VALUE2(1, 1);
	CHECK_VALUE2(1, 2);
	CHECK_VALUE2(2, 1);
	CHECK_VALUE2(2, 2);
	if (!zero_denied) {
		CHECK_VALUE2(0, 0);
	}
	CHECK_VALUE2(-1, -1);
	CHECK_VALUE2(0.1, 0.5);
	CHECK_VALUE2(1.9, 4.9);
#undef CHECK_VALUE2
	mat_world_free(world);
}

void test_common_value_unifunc(CuTest* tc, mat_op_def_t* op_def, bool zero_denied, double(* fn)(double)) {
	mat_world_t* world = mat_world_new();
	mat_world_put_op(world, op_def->name, op_def);
#define CHECK_VALUE1(a) test_common_assert_value(tc, world, fn(a), "%s[%f]", op_def->name, (double)a);
	CHECK_VALUE1(1);
	CHECK_VALUE1(2);
	CHECK_VALUE1(2);
	if (!zero_denied) {
		CHECK_VALUE1(0);
	}
	CHECK_VALUE1(-1);
	CHECK_VALUE1(0.1);
	CHECK_VALUE1(1.9);
#undef CHECK_VALUE1
	mat_world_free(world);
}

void test_common_assert_parse_failure(CuTest* tc, mat_world_t* w, const char* fmt, ...) {
	va_list ap;
	char buf[65536];
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	mat_parser_t* parser = mat_parser_new(w, buf);
	CuAssertPtrEquals(tc, NULL, mat_parser_parse(parser));
	CuAssertIntEquals(tc, MAT_ARG_COUNT_MISMATCH, mat_err_get_id());
	mat_parser_free(parser);
}

void test_common_assert_value(CuTest* tc, mat_world_t* w, double ex, const char* fmt, ...) {
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
	mat_error_t err = mat_op_calc_value(expr, t);
	CuAssertIntEquals_Msg(tc, buf, MAT_SUCCESS, err);
	CuAssertDblEquals_Msg(tc, buf, ex, mpq_get_d(t), 0.000001);
	mpq_clear(t);
	mat_expr_free(expr);
	mat_parser_free(parser);
}

mat_expr_t* parse_expression(CuTest* tc, mat_world_t* w, const char* fmt, ...) {
	va_list ap;
	char buf[65536];
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	mat_parser_t* parser = mat_parser_new(w, buf);
	mat_expr_t* expr = mat_parser_parse(parser);
	CuAssertPtrNotNullMsg(tc, buf, expr);
	mat_parser_free(parser);
	return expr;
}

mat_error_t check_expr_error(CuTest* tc, mat_world_t* w, const char* fmt, ...) {
	va_list ap;
	char buf[65536];
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	mat_expr_t* expr = parse_expression(tc, w, buf);

	mpq_t r;
	mpq_init(r);
	mat_error_t err = mat_op_calc_value(expr, r);
	mpq_clear(r);
	mat_expr_free(expr);
	return err;

}
