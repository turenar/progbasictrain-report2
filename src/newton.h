#pragma once

#include "error.h"
#include "fwddecl.h"
#include "compat/gmp_wrapper.h"

typedef struct mat_newton_param {
	mpq_t min;
	mpq_t max;
	mpq_t initial;
	mpq_t epsilon;
	int precision;
	int patience;
} mat_newton_param_t;
mat_error_t mat_newton_optimize(mat_world_t*, const mat_expr_t*, const mat_newton_param_t*, mpq_t out);
