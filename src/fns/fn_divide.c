#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"
#include "../world.h"


static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_error_t checker(mat_world_t* w, const mpq_t a, const mpq_t b);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t*);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_divide = {
		"Divide",
		2, 2,
		&mat_fn_common_show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out) {
	return mat_fn_common_apply_bifunction(w, expr, out, &checker, mpq_div);
}

static mat_error_t checker(mat_world_t* w, const mpq_t a, const mpq_t b) {
	UNUSED_VAR(w);
	UNUSED_VAR(a);
	if (mpq_sgn(b) == 0) {
		return mat_err_set(mat_world_get_error_info(w), MAT_DIVIDED_BY_ZERO);
	} else {
		return MAT_SUCCESS;
	}
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	mat_expr_t* df = mat_op_make_differential(w, args[0]);
	mat_expr_t* dg = mat_op_make_differential(w, args[1]);
	mat_expr_t* dividend = mat_fn_common_subtract(
			mat_fn_common_times(df, mat_expr_new_from(args[1])),
			mat_fn_common_times(mat_expr_new_from(args[0]), dg));
	mat_expr_t* divider = mat_fn_common_times(mat_expr_new_from(args[1]), mat_expr_new_from(args[1]));
	return mat_fn_common_divide(dividend, divider);
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	return mat_fn_common_divide(
			mat_op_simplify(w, args[0]),
			mat_op_simplify(w, args[1]));
}
