#include "config.inc.h"

#include <stdio.h>
#include "fns.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(const mat_expr_t*);
static mat_expr_t* simplify(const mat_expr_t*);

const mat_op_def_t mat_fn_const = {
		"__const",
		0, 0,
		&show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static void show_expression(const mat_expr_t* expr) {
	double d = mpq_get_d(expr->value.constant);
	printf("%g", d);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out) {
	mpq_set(out, expr->value.constant);
	return MAT_SUCCESS;
}

static mat_expr_t* make_differential(const mat_expr_t* expr) {
	UNUSED_VAR(expr);
	mpq_t c;
	mpq_init(c);
	mat_expr_t* e = mat_expr_new_const(c);
	mpq_clear(c);
	return e;
}

static mat_expr_t* simplify(const mat_expr_t* expr) {
	return mat_expr_new_from(expr);
}
