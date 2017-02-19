#include "config.inc.h"

#include <stdlib.h>
#include "fn_common.h"
#include "fns.h"
#include "../expr.h"

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t*);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_power = {
		"Power",
		2, 2,
		&mat_fn_common_show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_bifunction(w, expr, result, NULL, &mpfr_pow);
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.func.args;
	mat_expr_t* x = args[0];
	mat_expr_t* y = args[1];

	// y^x  ->  (y * x') * (x ^ (y-1)) + (y' * log x) * x^y
	return
			mat_fn_common_plus(
					mat_fn_common_times(
							mat_fn_common_times(
									mat_expr_new_from(y),
									mat_op_make_differential(w, x)),
							mat_expr_new_bi_args(
									&mat_fn_power,
									mat_expr_new_from(x),
									mat_fn_common_subtract(
											mat_expr_new_from(y),
											mat_expr_new_const_int(1)))),
					mat_fn_common_times(
							mat_fn_common_times(
									mat_op_make_differential(w, y),
									mat_expr_new_uni_arg(
											&mat_fn_log,
											mat_expr_new_from(x))),
							mat_expr_new_from(expr)));
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.func.args;
	mat_expr_t* a = mat_op_simplify(w, args[0]);
	mat_expr_t* b = mat_op_simplify(w, args[1]);
	if (mat_expr_is_const(b)) {
		if (mat_expr_is_const(a) && (mpq_sgn(a->value.constant) != 0 || mpq_sgn(b->value.constant) != 0)) {
			// 定数展開
			mpfr_t afr, bfr, rfr;
			mpf_t rf;
			mpq_t rq;

			mpfr_init(afr);
			mpfr_init(bfr);
			mpfr_init(rfr);
			mpf_init(rf);
			mpq_init(rq);

			mpfr_set_q(afr, a->value.constant, MPFR_RNDN);
			mpfr_set_q(bfr, b->value.constant, MPFR_RNDN);
			mpfr_pow(rfr, afr, bfr, MPFR_RNDN);
			mpfr_get_f(rf, rfr, MPFR_RNDN);
			mpq_set_f(rq, rf);

			mat_expr_t* expr = mat_expr_new_const(rq);

			mpfr_clear(afr);
			mpfr_clear(bfr);
			mpfr_clear(rfr);
			mpf_clear(rf);
			mpq_clear(rq);
			mat_expr_free(a);
			mat_expr_free(b);

			return expr;
		} else if (mpq_sgn(b->value.constant) == 0) {
			// 厳密にはa!=0である必要があるが、x^0 = 1としたい
			mat_expr_free(a);
			mat_expr_free(b);
			return mat_expr_new_const_int(1);
		} else if (mpq_cmp_ui(b->value.constant, 1, 1) == 0) {
			mat_expr_free(b);
			return a; // a^1 = a
		} else if (mat_expr_is_function(a) && a->op_def == &mat_fn_power) {
			// (x^2) ^3 -> x^6
			mat_expr_t* ab = a->value.func.args[1];
			if (mat_expr_is_const(ab)) {
				mat_expr_t* new_a = mat_expr_new_from(a->value.func.args[0]);
				mat_expr_t* new_b = mat_fn_common_times(b, mat_expr_new_from(ab));
				mat_expr_free(a);
				// b is moved in new_b assignment
				return mat_expr_new_bi_args(&mat_fn_power, new_a, new_b);
			}
		}
	}
	return mat_expr_new_bi_args(&mat_fn_power, a, b);
}
