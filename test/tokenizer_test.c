#include "config.inc.h"

#include "cutest_util.h"
#include "tokenizer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void check_tokenizer(CuTest* tc, mat_tokenizer_t* t, const char* s, mat_tokenizer_token_type_t ty,
                            size_t r, size_t c) {
	CuAssertIntEquals(tc, ty, mat_tokenizer_next(t));
	if (s) {
		CuAssertStrEquals(tc, s, mat_tokenizer_get_token(t));
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
	check_tokenizer(tc, t, "5", MAT_TOKEN_LITERAL, 1, 8);
	check_tokenizer(tc, t, "[", MAT_TOKEN_FUNC_OPENING_BRACKET, 1, 10);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 1, 12);
	check_tokenizer(tc, t, ",", MAT_TOKEN_FUNC_ARG_SEPARATOR, 1, 14);
	check_tokenizer(tc, t, NULL, MAT_TOKEN_END_OF_EXPRESSION, 1, 15);
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
			" , 3 ] , x ]\n";
	mat_tokenizer_t* t = mat_tokenizer_new(expr);

	CuAssertStrEquals(tc, expr, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "Plus", MAT_TOKEN_FUNC_NAME, 1, 1);

	check_tokenizer(tc, t, "[", MAT_TOKEN_FUNC_OPENING_BRACKET, 2, 1);
	CuAssertStrEquals(tc, expr + 5, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "Times", MAT_TOKEN_FUNC_NAME, 2, 3);
	check_tokenizer(tc, t, "[", MAT_TOKEN_FUNC_OPENING_BRACKET, 2, 9);

	check_tokenizer(tc, t, "Sin", MAT_TOKEN_FUNC_NAME, 3, 1);
	CuAssertStrEquals(tc, expr + 5 + 11, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "[", MAT_TOKEN_FUNC_OPENING_BRACKET, 3, 5);
	check_tokenizer(tc, t, "13.4", MAT_TOKEN_LITERAL, 3, 7);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 3, 12);

	check_tokenizer(tc, t, ",", MAT_TOKEN_FUNC_ARG_SEPARATOR, 4, 2);
	CuAssertStrEquals(tc, expr + 5 + 11 + 13, mat_tokenizer_get_row_str(t));
	check_tokenizer(tc, t, "3", MAT_TOKEN_LITERAL, 4, 4);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 4, 6);
	check_tokenizer(tc, t, ",", MAT_TOKEN_FUNC_ARG_SEPARATOR, 4, 8);
	check_tokenizer(tc, t, "x", MAT_TOKEN_VARIABLE, 4, 10);
	check_tokenizer(tc, t, "]", MAT_TOKEN_FUNC_CLOSING_BRACKET, 4, 12);

	check_tokenizer(tc, t, NULL, MAT_TOKEN_END_OF_EXPRESSION, 5, 1);
	CuAssertStrEquals(tc, "", mat_tokenizer_get_row_str(t));
	mat_tokenizer_free(t);
}

int main() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, &test_tokenizer_empty);
	SUITE_ADD_TEST(suite, &test_tokenizer_illegal_syntax);
	SUITE_ADD_TEST(suite, &test_tokenizer_unexpected_char);
	SUITE_ADD_TEST(suite, &test_tokenizer_get_str);
	SUITE_ADD_TEST(suite, &test_tokenizer_get_row_str);

	CuString* output = CuStringNew();
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	fflush(stdout);
	return suite->failCount != 0;

}
