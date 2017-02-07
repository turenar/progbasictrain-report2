#include "config.inc.h"

#include "fn_common.h"
#include "fns.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);
static mat_error_t checker(const mpq_t a, const mpq_t b);

mat_op_def_t mat_fn_divide = {
		"Divide",
		2, 2,
		&show_expression,
		&calc_value
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out) {
	return mat_fn_common_apply_bifunction(expr, out, &checker, mpq_div);
}

static mat_error_t checker(const mpq_t a, const mpq_t b) {
	UNUSED_VAR(a);
	if (mpq_sgn(b) == 0) {
		return MAT_DIVIDED_BY_ZERO;
	} else {
		return MAT_SUCCESS;
	}
}
