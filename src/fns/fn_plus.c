#include "config.inc.h"

#include "fns.h"
#include "../expr.h"
#include "../op.h"
#include "fn_common.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);

mat_op_def_t mat_fn_plus = {
		"Plus",
		&show_expression,
		&calc_value
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out) {
	return mat_fn_common_apply_bifunction(expr, out, mpq_add);
}
