#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t*);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_times = {
		"Times",
		2, 2,
		&mat_fn_common_show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out) {
	return mat_fn_common_apply_bifunction(w, expr, out, NULL, mpq_mul);
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	return mat_fn_common_plus(
			mat_fn_common_times(
					mat_op_make_differential(w, args[0]),
					mat_expr_new_from(args[1])),
			mat_fn_common_times(
					mat_expr_new_from(args[0]),
					mat_op_make_differential(w, args[1])));
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	return mat_fn_common_times(
			mat_op_simplify(w, args[0]),
			mat_op_simplify(w, args[1]));
}
