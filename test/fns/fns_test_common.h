#pragma once

#include "fns_test.h"

void test_common_arguments_bifunc(CuTest*, const mat_op_def_t* op_def);
void test_common_arguments_unifunc(CuTest*, const mat_op_def_t* op_def);
void test_common_value_bifunc(CuTest*, const mat_op_def_t*, bool zero_denied, double(* )(double, double));
void test_common_value_unifunc(CuTest*, const mat_op_def_t*, bool zero_denied, double(* )(double));

void test_common_assert_parse_failure(CuTest*, mat_world_t*, const char*, ...) __attribute_format__(printf, 3, 4);
void test_common_assert_value(CuTest*, mat_world_t*, double, const char*, ...) __attribute_format__(printf, 4, 5);
mat_expr_t* parse_expression(CuTest* tc, mat_world_t*, const char*, ...) __attribute_format__(printf, 3, 4);
mat_error_t check_expr_error(CuTest* tc, mat_world_t*, const char*, ...) __attribute_format__(printf, 3, 4);
