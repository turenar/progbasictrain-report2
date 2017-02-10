#include "config.inc.h"

#include <stdio.h>
#include "fns.h"
#include "../expr.h"
#include "../op.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);

const mat_op_def_t mat_fn_const = {
		"__const",
		0, 0,
		&show_expression,
		&calc_value
};

static void show_expression(const mat_expr_t* expr) {
	double d = mpq_get_d(expr->value.constant);
	printf("%f", d);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out) {
	mpq_set(out, expr->value.constant);
	return MAT_SUCCESS;
}
