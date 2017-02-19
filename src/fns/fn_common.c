#include "config.inc.h"

#include "fn_common.h"
#include <assert.h>
#include <stdio.h>
#include "fns.h"
#include "../expr.h"
#include "../op.h"
#include "../world.h"

void mat_fn_common_show_expression(mat_world_t* w, const mat_expr_t* expr) {
	assert(expr->op_type == MAT_OP_FUNCTION);

	printf("%s[", expr->op_def->name);
	mat_op_expr_t op_expr = expr->value.expr;
	for (unsigned int i = 0; i < op_expr.count; ++i) {
		if (i != 0) {
			printf(", ");
		}
		mat_expr_t* arg = op_expr.args[i];
		mat_op_show_expression(w, arg);
	}
	printf("]");
}

mat_error_t mat_fn_common_apply_bifunction(mat_world_t* w, const mat_expr_t* expr, mpq_t out,
                                           mat_bifunc_checker chk, mpq_bifunc fn) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t a;
	mpq_t b;
	mpq_init(a);
	mpq_init(b);
	mat_error_t t;
	t = mat_op_calc_value(w, args[0], a);
	if (t) { goto clean_up; }
	t = mat_op_calc_value(w, args[1], b);
	if (t) { goto clean_up; }

	if (chk) {
		t = chk(w, a, b);
		if (t) {
			goto clean_up;
		}
	}

	fn(out, a, b);
	t = MAT_SUCCESS;
clean_up:
	mpq_clear(a);
	mpq_clear(b);

	return t;
}

mat_error_t mat_fn_common_apply_mpfr_function(mat_world_t* w, const mat_expr_t* expr, mpq_t out,
                                              mat_func_checker chk, mpfr_func fn) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t arg;
	mpq_init(arg);

	mat_error_t t;
	t = mat_op_calc_value(w, args[0], arg);
	if (t) { goto clean_up; }

	if (chk) {
		t = chk(w, arg);
		if (t) {
			goto clean_up;
		}
	}

	mpfr_t in_fr;
	mpfr_t out_fr;
	mpfr_init(in_fr);
	mpfr_init(out_fr);
	mpfr_set_q(in_fr, arg, MPFR_RNDN);
	fn(out_fr, in_fr, MPFR_RNDN);

	mpf_t result_f;
	mpf_init(result_f);
	mpfr_get_f(result_f, out_fr, MPFR_RNDN);
	mpq_set_f(out, result_f);

	mpf_clear(result_f);
	mpfr_clear(out_fr);
	mpfr_clear(in_fr);
	t = MAT_SUCCESS;
clean_up:
	mpq_clear(arg);
	return t;
}

mat_error_t mat_fn_common_apply_mpfr_bifunction(mat_world_t* w, const mat_expr_t* expr, mpq_t out,
                                                mat_bifunc_checker chk, mpfr_bifunc fn) {
	mat_expr_t** args = expr->value.expr.args;
	mpq_t a_q;
	mpq_t b_q;
	mpq_init(a_q);
	mpq_init(b_q);

	mat_error_t t;

	t = mat_op_calc_value(w, args[0], a_q);
	if (t) { goto clean_up; }
	t = mat_op_calc_value(w, args[1], b_q);
	if (t) { goto clean_up; }

	if (chk) {
		t = chk(w, a_q, b_q);
		if (t) {
			goto clean_up;
		}
	}

	mpfr_t a_fr;
	mpfr_t b_fr;
	mpfr_t out_fr;
	mpfr_init(a_fr);
	mpfr_init(b_fr);
	mpfr_init(out_fr);
	mpfr_set_q(a_fr, a_q, MPFR_RNDN);
	mpfr_set_q(b_fr, b_q, MPFR_RNDN);
	fn(out_fr, a_fr, b_fr, MPFR_RNDN);

	mpf_t out_f;
	mpf_init(out_f);
	mpfr_get_f(out_f, out_fr, MPFR_RNDN);
	mpq_set_f(out, out_f);

	mpf_clear(out_f);
	mpfr_clear(out_fr);
	mpfr_clear(a_fr);
	mpfr_clear(b_fr);
	t = MAT_SUCCESS;
clean_up:
	mpq_clear(a_q);
	mpq_clear(b_q);
	return t;
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

mat_expr_t* mat_fn_common_plus(mat_expr_t* a, mat_expr_t* b) {
	if (mat_expr_is_const(a)) {
		if (mat_expr_is_const(b)) {
			mpq_t result;
			mpq_init(result);
			mpq_add(result, a->value.constant, b->value.constant);
			mat_expr_t* ret = mat_expr_new_const(result);
			mpq_clear(result);
			mat_expr_free(a);
			mat_expr_free(b);
			return ret;
		} else if (mpq_sgn(a->value.constant) == 0) {
			mat_expr_free(a);
			return b; // 0+b = 0
		}
	} else if (mat_expr_is_const(b)) {
		if (mpq_sgn(b->value.constant) == 0) {
			mat_expr_free(b);
			return a; // a+0 = a
		}
	}
	return mat_expr_new_bi_args(&mat_fn_plus, a, b);
}

mat_expr_t* mat_fn_common_subtract(mat_expr_t* a, mat_expr_t* b) {
	if (mat_expr_is_const(a)) {
		if (mat_expr_is_const(b)) {
			mpq_t result;
			mpq_init(result);
			mpq_sub(result, a->value.constant, b->value.constant);
			mat_expr_t* ret = mat_expr_new_const(result);
			mpq_clear(result);
			mat_expr_free(a);
			mat_expr_free(b);
			return ret;
		}
	} else if (mat_expr_is_const(b)) {
		if (mpq_sgn(b->value.constant) == 0) {
			mat_expr_free(b);
			return a; // a-0 = a
		}
	}
	return mat_expr_new_bi_args(&mat_fn_subtract, a, b);
}

mat_expr_t* mat_fn_common_times(mat_expr_t* a, mat_expr_t* b) {
	if (mat_expr_is_const(a)) {
		if (mat_expr_is_const(b)) {
			mpq_t result;
			mpq_init(result);
			mpq_mul(result, a->value.constant, b->value.constant);
			mat_expr_t* ret = mat_expr_new_const(result);
			mpq_clear(result);
			mat_expr_free(a);
			mat_expr_free(b);
			return ret;
		} else if (mpq_sgn(a->value.constant) == 0) {
			mat_expr_free(b);
			return a; // 0*b = 0
		} else if (mpq_cmp_si(a->value.constant, 1, 1) == 0) {
			mat_expr_free(a);
			return b; // 1*b = b
		}
	} else if (mat_expr_is_const(b)) {
		if (mpq_sgn(b->value.constant) == 0) {
			mat_expr_free(a);
			return b; // a*0 = 0
		} else if (mpq_cmp_si(b->value.constant, 1, 1) == 0) {
			mat_expr_free(b);
			return a; // a*1 = a
		}
	}
	return mat_expr_new_bi_args(&mat_fn_times, a, b);
}

mat_expr_t* mat_fn_common_divide(mat_expr_t* a, mat_expr_t* b) {
	if (mat_expr_is_const(a)) {
		if (mat_expr_is_const(b)) {
			mpq_t result;
			mpq_init(result);
			mpq_div(result, a->value.constant, b->value.constant);
			mat_expr_t* ret = mat_expr_new_const(result);
			mpq_clear(result);
			mat_expr_free(a);
			mat_expr_free(b);
			return ret;
		} else if (mpq_sgn(a->value.constant) == 0) {
			// 0/b is not always 0 (if b==0, undetermined), but should be 0?
			mat_expr_free(a);
			mat_expr_free(b);
			return mat_expr_new_const_int(0);
		}
	} else if (mat_expr_is_const(b)) {
		if (mpq_cmp_si(b->value.constant, 1, 1) == 0) {
			mat_expr_free(b);
			return a; // a/1 = a
		}
	}
	return mat_expr_new_bi_args(&mat_fn_divide, a, b);
}
