#include "config.inc.h"

#include <stdio.h>
#include "fns.h"
#include "../expr.h"
#include "../op.h"
#include "../world.h"

static void show_expression(mat_world_t* w, const mat_expr_t*);
static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out);
static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t*);
static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t*);

const mat_op_def_t mat_fn_variable = {
		"__variable",
		0, 0,
		&show_expression,
		&calc_value,
		&make_differential,
		&simplify
};

static void show_expression(mat_world_t* w, const mat_expr_t* expr) {
	UNUSED_VAR(w);
	printf("%c", expr->value.var);
}

static mat_error_t calc_value(mat_world_t* w, const mat_expr_t* expr, mpq_t out) {
	mat_expr_t* saved_variable = mat_world_get_variable(w, expr->value.var);
	if (saved_variable) {
		return mat_op_calc_value(w, saved_variable, out);
	} else {
		return mat_err_set_format(mat_world_get_error_info(w), MAT_HAVE_VARIABLE,
		                          "expression has unresolved variable `%c'", expr->value.var);
	}
}

static mat_expr_t* make_differential(mat_world_t* w, const mat_expr_t* expr) {
	UNUSED_VAR(w);
	if (expr->value.var == 'x') {
		return mat_expr_new_const_int(1);
	} else {
		return mat_expr_new_from(expr);
	}
}

static mat_expr_t* simplify(mat_world_t* w, const mat_expr_t* expr) {
	UNUSED_VAR(w);
	return mat_expr_new_from(expr);
}
