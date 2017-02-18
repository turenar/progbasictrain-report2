#include "config.inc.h"

#include <stdio.h>
#include "fns.h"
#include "../expr.h"
#include "../op.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(const mat_expr_t*);
static mat_expr_t* simplify(const mat_expr_t*);

const mat_op_def_t mat_fn_variable = {
		"__variable",
		0, 0,
		&show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static void show_expression(const mat_expr_t* expr) {
	printf("%c", expr->value.var);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out) {
	UNUSED_VAR(expr);
	UNUSED_VAR(out);
	return MAT_HAVE_VARIABLE;
}

static mat_expr_t* make_differential(const mat_expr_t* expr) {
	if (expr->value.var == 'x') {
		return mat_expr_new_const_int(1);
	} else {
		return mat_expr_new_from(expr);
	}
}

static mat_expr_t* simplify(const mat_expr_t* expr) {
	return mat_expr_new_from(expr);
}
