#include "config.inc.h"

#include <stdio.h>
#include "cutest_util.h"
#include "expr.h"
#include "op.h"
#include "parser.h"
#include "world.h"
#include "fns/fn_common.h"

static mat_error_t calc_value(const mat_expr_t*, mpq_t);

static mat_op_def_t test_op = {
		"Test",
		1, 2,
		&mat_fn_common_show_expression, &calc_value, NULL, NULL
};

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t r) {
	mat_expr_t* arg = expr->value.expr.args[0];
	mpq_t a, b;
	mpq_inits(a, b, NULL);
	mat_error_t err = mat_op_calc_value(arg, a);
	mpq_set_ui(b, 1, 1);
	mpq_add(r, a, b);
	mpq_clears(a, b, NULL);
	return err;
}

static void check_parse_failure(CuTest* tc, mat_error_t err, const char* e) {
	mat_world_t* w = mat_world_new();
	mat_world_put_op(w, test_op.name, &test_op);
	mat_parser_t* parser = mat_parser_new(w, e);
	CuAssertPtrEquals_Msg(tc, e, NULL, mat_parser_parse(parser));
	CuAssertIntEquals_Msg(tc, e, err, mat_err_get_id());
	mat_parser_free(parser);
	mat_world_free(w);
}

static void check_value(CuTest* tc, mat_error_t err, double r, const char* e) {
	mat_world_t* w = mat_world_new();
	mat_world_put_op(w, test_op.name, &test_op);
	mat_parser_t* parser = mat_parser_new(w, e);
	mat_expr_t* expr = mat_parser_parse(parser);
	CuAssertPtrNotNullMsg(tc, e, expr);
	mpq_t res;
	mpq_init(res);
	CuAssertIntEquals_Msg(tc, e, err, mat_op_calc_value(expr, res));
	if (!err) {
		CuAssertDblEquals_Msg(tc, e, r, mpq_get_d(res), 0.00001);
	}
	mpq_clear(res);
	mat_expr_free(expr);
	mat_parser_free(parser);
	mat_world_free(w);
}

static void test_parser_illegal_syntax(CuTest* tc) {
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, ""); // empty
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "@"); // illegal char
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test, X");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[x");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[x 5]");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[x,");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[Test[");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[Test]");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[Test[1]");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "Test[,]");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "4..2");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, ".");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "[");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, "]");
	check_parse_failure(tc, MAT_PARSER_UNEXPECTED_TOKEN, ",");
}

static void test_parser_unknown_func(CuTest* tc) {
	check_parse_failure(tc, MAT_UNKNOWN_FUNC, "Unknown[]");
	check_parse_failure(tc, MAT_UNKNOWN_FUNC, "tesT[]");
}

static void test_parser_arg_count_mismatch(CuTest* tc) {
	check_parse_failure(tc, MAT_ARG_COUNT_MISMATCH, "Test[]");
	check_parse_failure(tc, MAT_ARG_COUNT_MISMATCH, "Test[1,1,1]");
	check_parse_failure(tc, MAT_ARG_COUNT_MISMATCH, "Test[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]");
}

static void test_parser_check_value(CuTest* tc) {
	check_value(tc, MAT_SUCCESS, 2, "Test[1]");
	check_value(tc, MAT_HAVE_VARIABLE, 0, "Test[x]");
	check_value(tc, MAT_HAVE_VARIABLE, 0, "Test[x,1]");
	check_value(tc, MAT_SUCCESS, 3, "Test[Test[1]]");
}

int main() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_parser_illegal_syntax);
	SUITE_ADD_TEST(suite, &test_parser_unknown_func);
	SUITE_ADD_TEST(suite, &test_parser_arg_count_mismatch);
	SUITE_ADD_TEST(suite, &test_parser_check_value);

	CuString* output = CuStringNew();
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	fflush(stdout);
	return suite->failCount != 0;
}
