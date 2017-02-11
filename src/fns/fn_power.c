#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"
#include "../expr.h"
#include <stdlib.h>

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(const mat_expr_t*);

const mat_op_def_t mat_fn_power = {
		"Power",
		2, 2,
		&show_expression,
		&calc_value,
		&make_differential
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_bifunction(expr, result, NULL, &mpfr_pow);
}

static mat_expr_t* make_differential(const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	mat_expr_t* x = args[0];
	mat_expr_t* y = args[1];

	// y^x  ->  (y * x') * (x ^ (y-1)) + (y' * log x) * x^y
	return
			mat_fn_common_add(
					mat_fn_common_multiply(
							mat_fn_common_multiply(
									mat_expr_new_from(y),
									mat_op_make_differential(x)),
							mat_expr_new_bi_args(
									&mat_fn_power,
									mat_expr_new_from(x),
									mat_fn_common_subtract(
											mat_expr_new_from(y),
											mat_expr_new_const_double(1)))),
					mat_fn_common_multiply(
							mat_fn_common_multiply(
									mat_op_make_differential(y),
									mat_expr_new_uni_arg(
											&mat_fn_log,
											mat_expr_new_from(x))),
							mat_expr_new_from(expr)));
}
