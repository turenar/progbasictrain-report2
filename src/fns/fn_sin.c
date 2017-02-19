#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_sin = {
		"Sin",
		1, 1,
		&mat_fn_common_show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_function(w, expr, result, NULL, &mpfr_sin);
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t* b = mat_expr_new_from(expr);
	b->op_def = &mat_fn_cos;
	return mat_fn_common_times(
			mat_op_make_differential(w, expr->value.func.args[0]),
			b);
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	UNUSED_VAR(w);
	return mat_expr_new_from(expr);
}
