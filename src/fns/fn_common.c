#include "config.inc.h"

#include "fn_common.h"
#include <stdio.h>
#include "assert.h"
#include "../expr.h"
#include "../op.h"
#include "../world.h"
#include "fns.h"

void mat_fn_common_show_expression(const mat_expr_t* expr) {
	assert(expr->op_id >= 0);

	printf("%s[", expr->op_def->name);
	mat_op_expr_t op_expr = expr->value.expr;
	for (unsigned int i = 0; i < op_expr.count; ++i) {
		if (i != 0) {
			printf(", ");
		}
		mat_expr_t* arg = op_expr.args[i];
		arg->op_def->show_expression(arg);
	}
	printf("]");
}

mat_error_t mat_fn_common_apply_bifunction(const mat_expr_t* expr, mpq_t out, mpq_bifunc fn) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t a = {0};
	mpq_t b = {0};
	mpq_init(a);
	mpq_init(b);
	args[0]->op_def->calc_value(args[0], a);
	args[1]->op_def->calc_value(args[1], b);

	fn(out, a, b);
	mpq_clear(a);
	mpq_clear(b);

	return MAT_SUCCESS;
}

mat_error_t mat_fn_common_apply_mpfr_function(const mat_expr_t* expr, mpq_t out, mpfr_func fn) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t arg = {0};
	mpq_init(arg);
	args[0]->op_def->calc_value(args[0], arg);

	mpfr_t in_fr = {0};
	mpfr_t out_fr = {0};
	mpfr_init(in_fr);
	mpfr_init(out_fr);
	mpfr_set_q(in_fr, arg, MPFR_RNDN);
	fn(out_fr, in_fr, MPFR_RNDN);

	mpf_t result_f = {0};
	mpf_init(result_f);
	mpfr_get_f(result_f, out_fr, MPFR_RNDN);
	mpq_set_f(out, result_f);

	mpq_clear(arg);
	mpfr_clear(in_fr);
	mpfr_clear(out_fr);
	mpf_clear(result_f);
	return MAT_SUCCESS;
}

mat_error_t mat_fn_common_apply_mpfr_bifunction(const mat_expr_t* expr, mpq_t out, mpfr_bifunc fn) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t a_q = {0};
	mpq_t b_q = {0};
	mpq_init(a_q);
	mpq_init(b_q);
	args[0]->op_def->calc_value(args[0], a_q);
	args[1]->op_def->calc_value(args[1], b_q);

	mpfr_t a_fr = {0};
	mpfr_t b_fr = {0};
	mpfr_t out_fr = {0};
	mpfr_init(a_fr);
	mpfr_init(b_fr);
	mpfr_init(out_fr);
	mpfr_set_q(a_fr, a_q, MPFR_RNDN);
	mpfr_set_q(b_fr, b_q, MPFR_RNDN);
	fn(out_fr, a_fr, b_fr, MPFR_RNDN);

	mpf_t out_f = {0};
	mpf_init(out_f);
	mpfr_get_f(out_f, out_fr, MPFR_RNDN);
	mpq_set_f(out, out_f);

	mpq_clear(a_q);
	mpq_clear(b_q);
	mpfr_clear(a_fr);
	mpfr_clear(b_fr);
	mpfr_clear(out_fr);
	mpf_clear(out_f);

	return MAT_SUCCESS;
}

void mat_fn_put_stdfunc(mat_world_t* w) {
#define PUT_OP(var) mat_world_put_op(w, var.name, &var);
	PUT_OP(mat_fn_plus);
	PUT_OP(mat_fn_subtract);
	PUT_OP(mat_fn_times);
	PUT_OP(mat_fn_divide);
	PUT_OP(mat_fn_sin);
	PUT_OP(mat_fn_cos);
	PUT_OP(mat_fn_exp);
	PUT_OP(mat_fn_log);
	PUT_OP(mat_fn_power);
#undef PUT_OP
}
