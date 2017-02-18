#include "config.inc.h"

#include "expr.h"
#include "fn_common.h"
#include "fns.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(const mat_expr_t*);
static mat_expr_t* simplify(const mat_expr_t*);

const mat_op_def_t mat_fn_cos = {
		"Cos",
		1, 1,
		&show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t result) {
	return mat_fn_common_apply_mpfr_function(expr, result, NULL, &mpfr_cos);
}

static mat_expr_t* make_differential(const mat_expr_t* expr) {
	mat_expr_t* a = mat_op_make_differential(expr->value.expr.args[0]);
	mat_expr_t* minus = mat_expr_new_const_int(-1);
	mat_expr_t* b = mat_expr_new_from(expr);
	b->op_def = &mat_fn_sin;
	return mat_fn_common_multiply(mat_fn_common_multiply(minus, a), b);
}

static mat_expr_t* simplify(const mat_expr_t* expr) {
	return mat_expr_new_from(expr);
}
