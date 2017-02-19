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
	mat_expr_t** args = expr->value.func.args;
	mat_expr_t* a = args[0];
	mat_expr_t* b = args[1];
	return
			mat_fn_common_divide(
					mat_fn_common_subtract(
							mat_fn_common_times(
									mat_op_make_differential(w, a),
									mat_expr_new_from(b)),
							mat_fn_common_times(
									mat_expr_new_from(a),
									mat_op_make_differential(w, b))),
					mat_expr_new_bi_args(
							&mat_fn_power,
							mat_expr_new_from(b),
							mat_expr_new_const_int(2)));
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.func.args;
	return mat_fn_common_divide(
			mat_op_simplify(w, args[0]),
			mat_op_simplify(w, args[1]));
}
