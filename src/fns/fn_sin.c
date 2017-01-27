#include "config.inc.h"

#include "fns.h"
#include <mpfr.h>
#include "../expr.h"
#include "../op.h"
#include "fn_common.h"

static void show_expression(const mat_expr_t*);
static mat_error_t calc_value(const mat_expr_t* expr, mpq_t out);

mat_op_def_t mat_fn_sin = {
		"Sin",
		&show_expression,
		&calc_value
};

static void show_expression(const mat_expr_t* expr) {
	mat_fn_common_show_expression(expr);
}

static mat_error_t calc_value(const mat_expr_t* expr, mpq_t result) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t a = {0};
	mpq_init(a);
	args[0]->op_def->calc_value(args[0], a);

	mpfr_t in = {0};
	mpfr_t out = {0};
	mpfr_init(in);
	mpfr_init(out);
	mpfr_set_q(in, a, MPFR_RNDN);
	mpfr_sin(out, in, MPFR_RNDN);

	mpf_t result_f = {0};
	mpf_init(result_f);
	mpfr_get_f(result_f, out, MPFR_RNDN);
	mpq_set_f(result, result_f);

	mpq_clear(a);
	mpfr_clear(in);
	mpfr_clear(out);
	mpf_clear(result_f);
	return MAT_SUCCESS;
}
