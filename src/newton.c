#include "config.inc.h"

#include "newton.h"
#include <stddef.h>
#include "expr.h"
#include "logger.h"
#include "op.h"
#include "world.h"


// mpq_tだけだとxの動きが小さい (傾きの絶対値が大きいときに) 分母分子が大きくなってしまい、永遠と時間がかかってしまうため、
// それを防ぐときには0にする。precisionパラメータが導入されたことにより、xの動きが小さくてもmpq_t→精度限定mpf_t→mpq_tの変換を経ることで
// 精度を犠牲に一定のスピードで動くようになる。ニュートン法はそもそも解析解であり厳密解ではないので
// 一定以上の精度は必要ないはず。
#define USE_DOUBLE_AS_PRECISION_LIMIT 0

static mat_error_t optimize(mat_world_t* w, const mat_expr_t* fx, const mat_expr_t* df,
                            const mat_newton_param_t* param, mpq_t out);
static void drop_precision(mpq_t x, mpf_t tmp_f);
static bool is_too_small(const mpq_t a, const mpq_t epsilon, mpq_t tmp);

mat_error_t mat_newton_optimize(mat_world_t* w, const mat_expr_t* f, const mat_newton_param_t* param, mpq_t out) {
	mat_error_t r;
	mat_expr_t* df = mat_op_make_differential(w, f);
	if (mat_expr_is_const(df) && mpq_sgn(df->value.constant) == 0) {
		r = mat_err_set_format(mat_world_get_error_info(w), MAT_FAILURE, "const function!");
		goto clean_df;
	}

	LOG(debug, "range=[%f,%f], initial=%f, epsilon=%f, patience=%d", mpq_get_d(param->min), mpq_get_d(param->max),
	    mpq_get_d(param->initial), mpq_get_d(param->epsilon), param->patience);
	mat_expr_t* previous_expr;
	previous_expr = mat_world_get_variable(w, 'x'); // 一応xの式を保存しておく
	r = optimize(w, f, df, param, out);
	mat_world_put_variable(w, 'x', previous_expr); // 元に戻す
clean_df:
	mat_expr_free(df);
	return r;
}

static mat_error_t optimize(mat_world_t* w, const mat_expr_t* f, const mat_expr_t* df,
                            const mat_newton_param_t* param, mpq_t out) {
	mpq_t x, mdx, fx, dfx, tmp;
	mpf_t tmp_f;
	mat_error_t r;
	mpq_inits(x, mdx, fx, dfx, tmp, NULL);
	mpf_init2(tmp_f, (mp_bitcnt_t) param->precision);
	mpq_set(x, param->initial);

	int life = param->patience;
	do {
#if USE_DOUBLE_AS_PRECISION_LIMIT
		mpq_set_d(x, mpq_get_d(x) - mpq_get_d(mdx));
		(void) drop_precision;
#else
		mpq_sub(x, x, mdx);
		drop_precision(x, tmp_f);
#endif
		if (mpq_cmp(x, param->min) < 0) {
			r = MAT_FAILURE;
			mat_err_set_format(mat_world_get_error_info(w), r, "numeric limit exceeded (min=%f, x=%f)",
			                   mpq_get_d(param->min), mpq_get_d(x));
			goto clean_mpq;
		} else if (mpq_cmp(x, param->max) > 0) {
			r = MAT_FAILURE;
			mat_err_set_format(mat_world_get_error_info(w), r, "numeric limit exceeded (max=%f, x=%f)",
			                   mpq_get_d(param->max), mpq_get_d(x));
			goto clean_mpq;
		} else if (--life < 0) {
			r = MAT_FAILURE;
			mat_err_set_format(mat_world_get_error_info(w), r, "loop limit exceeded (patience=%d, x=%f)",
			                   param->patience, mpq_get_d(x));
			goto clean_mpq;
		}

		// 計算に必要
		mat_world_put_variable(w, 'x', mat_expr_new_const(x));
		r = mat_op_calc_value(w, f, fx);
		if (r) {
			goto clean_mpq;
		}
		r = mat_op_calc_value(w, df, dfx);
		if (r) {
			goto clean_mpq;
		}
		mpq_div(mdx, fx, dfx);
		LOG(debug, "x=%f, f(x)=%f, df(x)=%f, -dx=%f, patience=%d", mpq_get_d(x), mpq_get_d(fx), mpq_get_d(dfx),
		    mpq_get_d(mdx), life);
	} while (!is_too_small(mdx, param->epsilon, tmp));

	mpq_set(out, x);
	r = MAT_SUCCESS;
clean_mpq:
	mpq_clears(x, mdx, fx, dfx, tmp, NULL);
	mpf_clear(tmp_f);
	return r;
}

static bool is_too_small(const mpq_t a, const mpq_t epsilon, mpq_t tmp) {
	mpq_abs(tmp, a);
	LOG(debug, "a=%f, epsilon=%f, tmp=%f", mpq_get_d(a), mpq_get_d(epsilon), mpq_get_d(tmp));
	return mpq_cmp(tmp, epsilon) <= 0;
}

static void drop_precision(mpq_t x, mpf_t tmp_f) {
	// tmp_fはprecisionが指定されており、ある程度の精度までしか保存しない。変換、逆変換を行うことで大きな分母分子を持つ
	// 計算を行わないようにする
	mpf_set_q(tmp_f, x);
	mpq_set_f(x, tmp_f);
}
