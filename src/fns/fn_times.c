#include "config.inc.h"

#include "fns.h"
#include "../expr.h"
#include "../op.h"
#include "fn_common.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);

mat_op_def_t mat_fn_times = {
		"Times",
		&show_expression,
		&calc_value
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t a = {0};
	mpq_t b = {0};
	mpq_init(a);
	mpq_init(b);
	args[0]->op_def->calc_value(args[0], a);
	args[1]->op_def->calc_value(args[1], b);
	mpq_mul(out, a, b);
	mpq_clear(a);
	mpq_clear(b);
	return MAT_SUCCESS;
}
