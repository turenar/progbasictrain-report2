#include "config.inc.h"

#include "cutest_util.h"
#include "tokenizer.h"
#include <string.h>
#include <stdlib.h>

static void check_tokenizer(CuTest* tc, mat_tokenizer_t* t, const char* s, mat_tokenizer_token_type_t ty,
                            size_t r, size_t c) {
	CuAssertIntEquals(tc, ty, mat_tokenizer_next(t));
	if (s) {
		CuAssertStrEquals(tc, s, mat_tokenizer_get_str(t));
		char* dup = mat_tokenizer_dup_token(t);
		CuAssertStrEquals(tc, s, dup);
		CuAssertSizeTEquals(tc, strlen(s), mat_tokenizer_get_token_len(t));
		free(dup);
	}
	CuAssertSizeTEquals(tc, r, mat_tokenizer_get_row(t));
	CuAssertSizeTEquals(tc, c, mat_tokenizer_get_col(t));
}

static void test_tokenizer_empty(CuTest* tc) {
	mat_tokenizer_t* t = mat_tokenizer_new("");
	check_tokenizer(tc, t, NULL, MAT_TOKEN_END_OF_EXPRESSION, 1, 1);
	mat_tokenizer_free(t);
}

static void test_tokenizer_illegal_syntax(CuTest* tc) {
	// tokenizer do not throw syntax error because no state
	mat_tokenizer_t* t = mat_tokenizer_new("Assert 5 [ ] ,");
	check_tokenizer(tc, t, "Assert", MAT_TOKEN_FUNC_NAME, 1, 1);
	check_tokenizer(tc, t, "5", MAT_TOKEN_LITERAL, 1, 7);
	check_tokenizer(tc, t, "[", MAT_TOKEN_FUNC_OPENING_BRACKET, 1, 9);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 1, 11);
	check_tokenizer(tc, t, ",", MAT_TOKEN_FUNC_ARG_SEPARATOR, 1, 13);
	check_tokenizer(tc, t, NULL, MAT_TOKEN_END_OF_EXPRESSION, 1, 14);
	mat_tokenizer_free(t);
}

static void test_tokenizer_unexpected_char(CuTest* tc) {
	mat_tokenizer_t* t = mat_tokenizer_new("@");
	check_tokenizer(tc, t, NULL, MAT_TOKEN_UNKNOWN, 1, 1);
	mat_tokenizer_free(t);
}

static void test_tokenizer_get_str(CuTest* tc) {
	const char* expr = "Plus [ Times [ Sin [ 13 . 4 ] , 3 ] , 2 ]";
	mat_tokenizer_t* t = mat_tokenizer_new(expr);
	CuAssertStrEquals(tc, expr, mat_tokenizer_get_str(t));
	mat_tokenizer_free(t);
}

static void test_tokenizer_get_row_str(CuTest* tc) {
	const char* expr = "Plus\n"
			"[ Times\t[ \n"
			"Sin [ 13.4 ]\n"
			" , 3 ] , 2 ]";
	mat_tokenizer_t* t = mat_tokenizer_new(expr);

	CuAssertStrEquals(tc, expr, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "Plus", MAT_TOKEN_FUNC_NAME, 1, 1);

	check_tokenizer(tc, t, "[", MAT_TOKEN_FUNC_OPENING_BRACKET, 2, 1);
	CuAssertStrEquals(tc, expr + 5, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "Times", MAT_TOKEN_FUNC_NAME, 2, 3);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 2, 9);

	check_tokenizer(tc, t, "Sin", MAT_TOKEN_FUNC_NAME, 3, 1);
	CuAssertStrEquals(tc, expr + 5 + 11, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "[", MAT_TOKEN_FUNC_OPENING_BRACKET, 3, 5);
	check_tokenizer(tc, t, "13.4", MAT_TOKEN_LITERAL, 3, 7);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 3, 9);

	check_tokenizer(tc, t, ",", MAT_TOKEN_FUNC_ARG_SEPARATOR, 3, 1);
	CuAssertStrEquals(tc, expr + 5 + 11 + 10, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "3", MAT_TOKEN_LITERAL, 3, 5);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 3, 9);
	check_tokenizer(tc, t, ",", MAT_TOKEN_FUNC_ARG_SEPARATOR, 3, 1);
	check_tokenizer(tc, t, "2", MAT_TOKEN_LITERAL, 3, 5);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 3, 9);

	check_tokenizer(tc, t, NULL, MAT_TOKEN_END_OF_EXPRESSION, 3, 9);
	CuAssertStrEquals(tc, "", mat_tokenizer_get_row_str(t));
	mat_tokenizer_free(t);
}

int main() {
	CuSuite* ts = CuSuiteNew();
	SUITE_ADD_TEST(ts, &test_tokenizer_empty);
	SUITE_ADD_TEST(ts, &test_tokenizer_illegal_syntax);
	SUITE_ADD_TEST(ts, &test_tokenizer_unexpected_char);
	SUITE_ADD_TEST(ts, &test_tokenizer_get_str);
	SUITE_ADD_TEST(ts, &test_tokenizer_get_row_str);

}
