#pragma once

#include <cutest/CuTest.h>
#include "fns/fns.h"

CuSuite* test_fn_plus();
CuSuite* test_fn_subtract();
CuSuite* test_fn_times();
CuSuite* test_fn_divide();

CuSuite* test_fn_cos();
CuSuite* test_fn_sin();

CuSuite* test_fn_power();
CuSuite* test_fn_exp();
CuSuite* test_fn_log();
