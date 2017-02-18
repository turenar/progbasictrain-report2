#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"
#include "../world.h"

static void show_expression(mat_world_t* w, const mat_expr_t*);
static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_error_t checker(mat_world_t* w, const mpq_t a);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t*);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_log = {
		"Log",
		1, 1,
		&show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static void show_expression(mat_world_t* w, const mat_expr_t* expr) {
	mat_fn_common_show_expression(w, expr);
}

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_function(w, expr, result, &checker, &mpfr_log);
}

static mat_error_t checker(mat_world_t* w, const mpq_t a) {
	UNUSED_VAR(w);
	if (mpq_sgn(a) <= 0) {
		mat_err_set_format(MAT_ARITHMETIC_ERROR, "Log[x] cannot be x<=0 but %f", mpq_get_d(a));
		return MAT_ARITHMETIC_ERROR;
	} else {
		return MAT_SUCCESS;
	}
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t* arg = expr->value.expr.args[0];
	return mat_fn_common_divide(
			mat_op_make_differential(w, arg),
			mat_expr_new_from(arg));
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	mat_expr_t* arg = mat_op_simplify(w, expr->value.expr.args[0]);
	if (mat_expr_is_const(arg) && mpq_cmp_ui(arg->value.constant, 1, 1) == 0) {
		mat_expr_free(arg);
		return mat_expr_new_const_int(0);
	} else {
		return mat_expr_new_uni_arg(&mat_fn_log, arg);
	}
}
