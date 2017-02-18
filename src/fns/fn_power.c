#include "config.inc.h"

#include <stdlib.h>
#include "fn_common.h"
#include "fns.h"
#include "../expr.h"

static void show_expression(mat_world_t* w, const mat_expr_t*);
static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t*);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_power = {
		"Power",
		2, 2,
		&show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static void show_expression(mat_world_t* w, const mat_expr_t* expr) {
	mat_fn_common_show_expression(w, expr);
}

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_bifunction(w, expr, result, NULL, &mpfr_pow);
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	mat_expr_t* x = args[0];
	mat_expr_t* y = args[1];

	// y^x  ->  (y * x') * (x ^ (y-1)) + (y' * log x) * x^y
	return
			mat_fn_common_add(
					mat_fn_common_multiply(
							mat_fn_common_multiply(
									mat_expr_new_from(y),
									mat_op_make_differential(w, x)),
							mat_expr_new_bi_args(
									&mat_fn_power,
									mat_expr_new_from(x),
									mat_fn_common_subtract(
											mat_expr_new_from(y),
											mat_expr_new_const_int(1)))),
					mat_fn_common_multiply(
							mat_fn_common_multiply(
									mat_op_make_differential(w, y),
									mat_expr_new_uni_arg(
											&mat_fn_log,
											mat_expr_new_from(x))),
							mat_expr_new_from(expr)));
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	mat_expr_t* a = mat_op_simplify(w, args[0]);
	mat_expr_t* b = mat_op_simplify(w, args[1]);
	if (mat_expr_is_const(b)) {
		if (mat_expr_is_const(a) && mpq_sgn(a->value.constant) == 0 && mpq_sgn(b->value.constant) != 0) {
			mat_expr_free(a);
			mat_expr_free(b);
			return mat_expr_new_const_int(0);
		} else if (mpq_sgn(b->value.constant) == 0) {
			// 厳密にはa!=0である必要があるが、x^0 = 1としたい
			mat_expr_free(a);
			mat_expr_free(b);
			return mat_expr_new_const_int(1);
		} else if (mpq_cmp_ui(b->value.constant, 1, 1) == 0) {
			mat_expr_free(b);
			return a;
		}
	}
	return mat_expr_new_bi_args(&mat_fn_power, a, b);
}
