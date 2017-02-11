#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_error_t checker(const mpq_t a);
static mat_expr_t* make_differential(const mat_expr_t*);

const mat_op_def_t mat_fn_log = {
		"Log",
		1, 1,
		&show_expression,
		&calc_value,
		&make_differential
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_function(expr, result, &checker, &mpfr_log);
}

static mat_error_t checker(const mpq_t a) {
	if (mpq_sgn(a) <= 0) {
		mat_err_set_format(MAT_ARITHMETIC_ERROR, "Log[x] cannot be x<=0 but %f", mpq_get_d(a));
		return MAT_ARITHMETIC_ERROR;
	} else {
		return MAT_SUCCESS;
	}
}

static mat_expr_t* make_differential(const mat_expr_t* expr) {
	mat_expr_t* arg = expr->value.expr.args[0];
	return mat_fn_common_divide(
			mat_op_make_differential(arg),
			mat_expr_new_from(arg));
}
