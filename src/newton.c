#include "config.inc.h"

#include "newton.h"
#include "op.h"
#include "expr.h"
#include "world.h"
#include "logger.h"

static mat_error_t optimize(mat_world_t* w, const mat_expr_t* fx, const mat_expr_t* df,
                            const mpq_t initial, const mpq_t epsilon, mpq_t out);
static bool is_too_small(const mpq_t a, const mpq_t epsilon, mpq_t tmp);

mat_error_t mat_newton_optimize(mat_world_t* w, const mat_expr_t* f, const mpq_t initial, const mpq_t epsilon,
                                mpq_t out) {
	mat_error_t r;
	mat_expr_t* df = mat_op_make_differential(w, f);
	if (mat_expr_is_const(df) && mpq_sgn(df->value.constant) == 0) {
		r = MAT_FAILURE;
		mat_err_set_format(mat_world_get_error_info(w), r, "const function!");
		goto clean_df;
	}

	mat_expr_t* previous_expr;
	previous_expr = mat_world_get_variable(w, 'x');
	r = optimize(w, f, df, initial, epsilon, out);
	mat_world_put_variable(w, 'x', previous_expr);
clean_df:
	mat_expr_free(df);
	return r;
}

static mat_error_t optimize(mat_world_t* w, const mat_expr_t* f, const mat_expr_t* df,
                            const mpq_t initial, const mpq_t epsilon, mpq_t out) {
	mpq_t x, mdx, fx, dfx, tmp;
	mat_error_t r;
	mpq_inits(x, mdx, fx, dfx, tmp, NULL);
	mpq_set(x, initial);

	do {
		mpq_sub(x, x, mdx);
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
		LOG(debug, "x=%f, f(x)=%f, df(x)=%f, -dx=%f", mpq_get_d(x), mpq_get_d(fx), mpq_get_d(dfx), mpq_get_d(mdx));
	} while (!is_too_small(mdx, epsilon, tmp));

	mpq_set(out, x);
	r = MAT_SUCCESS;
clean_mpq:
	mpq_clears(x, mdx, fx, dfx, tmp, NULL);
	return r;
}

static bool is_too_small(const mpq_t a, const mpq_t epsilon, mpq_t tmp) {
	mpq_abs(tmp, a);
	LOG(debug, "a=%f, epsilon=%f, tmp=%f", mpq_get_d(a), mpq_get_d(epsilon), mpq_get_d(tmp));
	return mpq_cmp(tmp, epsilon) <= 0;
}
