#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(const mat_expr_t*);

const mat_op_def_t mat_fn_exp = {
		"Exp",
		1, 1,
		&show_expression,
		&calc_value,
		&make_differential
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_function(expr, result, NULL, &mpfr_exp);
}

static mat_expr_t* make_differential(const mat_expr_t* expr) {
	mat_expr_t* arg = expr->value.expr.args[0];
	return mat_fn_common_multiply(
			mat_op_make_differential(arg),
			mat_expr_new_from(expr));
}
