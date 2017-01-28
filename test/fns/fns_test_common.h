#pragma once

#include "fns_test.h"

void test_common_arguments_bifunc(CuTest*, mat_op_def_t* op_def);
void test_common_value_bifunc(CuTest*, mat_op_def_t*, bool zero_denied, double(* )(double, double));

