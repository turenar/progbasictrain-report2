#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t*);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_exp = {
		"Exp",
		1, 1,
		&mat_fn_common_show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_function(w, expr, result, NULL, &mpfr_exp);
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t* arg = expr->value.expr.args[0];
	return mat_fn_common_times(
			mat_op_make_differential(w, arg),
			mat_expr_new_from(expr));
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t* arg = mat_op_simplify(w, expr->value.expr.args[0]);
	if (mat_expr_is_const(arg) && mpq_sgn(arg->value.constant) == 0) {
		mat_expr_free(arg);
		return mat_expr_new_const_int(1);
	} else {
		return mat_expr_new_uni_arg(&mat_fn_exp, arg);
	}
}
