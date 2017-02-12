#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(const mat_expr_t*);
static mat_expr_t* simplify(const mat_expr_t*);

const mat_op_def_t mat_fn_times = {
		"Times",
		2, 2,
		&show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out) {
	return mat_fn_common_apply_bifunction(expr, out, NULL, mpq_mul);
}

static mat_expr_t* make_differential(const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	return mat_fn_common_add(
			mat_fn_common_multiply(
					mat_op_make_differential(args[0]),
					mat_expr_new_from(args[1])),
			mat_fn_common_multiply(
					mat_expr_new_from(args[0]),
					mat_op_make_differential(args[1])));
}

static mat_expr_t* simplify(const mat_expr_t* expr) {
	mat_expr_t** args = expr->value.expr.args;
	return mat_fn_common_multiply(
			mat_op_simplify(args[0]),
			mat_op_simplify(args[1]));
}
