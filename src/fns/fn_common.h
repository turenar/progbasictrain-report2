#pragma once

#include <stddef.h>
#include "../error.h"
#include "../fwddecl.h"
#include "compat/gmp_wrapper.h"
#include "compat/mpfr_wrapper.h"

/**
 * Name[Arg, Arg...] の形で出力する。
 * @param w worldインスタンス
 * @param expr 有効な式 (funcである必要が有ります)
 */
void mat_fn_common_show_expression(mat_world_t* w, const mat_expr_t* expr);

typedef mat_error_t (* mat_bifunc_checker)(mat_world_t* w, mpq_srcptr, mpq_srcptr);
typedef mat_error_t (* mat_func_checker)(mat_world_t* w, mpq_srcptr);

typedef void (* mpq_bifunc)(mpq_ptr, mpq_srcptr, mpq_srcptr);
mat_error_t mat_fn_common_apply_bifunction(mat_world_t* w, const mat_expr_t*, mpq_t out,
                                           mat_bifunc_checker, mpq_bifunc);

typedef int (* mpfr_func)(mpfr_ptr, mpfr_srcptr, mpfr_rnd_t);
mat_error_t mat_fn_common_apply_mpfr_function(mat_world_t* w, const mat_expr_t*, mpq_t out,
                                              mat_func_checker, mpfr_func);

typedef int (* mpfr_bifunc)(mpfr_ptr, mpfr_srcptr, mpfr_srcptr, mpfr_rnd_t);
mat_error_t mat_fn_common_apply_mpfr_bifunction(mat_world_t* w, const mat_expr_t*, mpq_t out,
                                                mat_bifunc_checker, mpfr_bifunc);

/**
 * Plus[A, B] 式を作成する。
 *
 * 可能ならば定数展開するため、実際に返る式がPlus関数であることは保証されない。
 * @param a ポインタの所有権を受け渡す必要がある (渡したポインタの直接freeや他の式の引数として利用などはしてはいけない)
 * @param b aと同様である。
 * @return 生成された式。NULLは返らない。
 */
mat_expr_t* mat_fn_common_plus(mat_expr_t* a, mat_expr_t* b);
/**
 * Subtract[A, B] 式を作成する。
 * @see mat_fn_common_plus
 * @return 生成された式。NULLは返らない。
 */
mat_expr_t* mat_fn_common_subtract(mat_expr_t*, mat_expr_t*);
/**
 * Times[A, B] 式を作成する。
 * @see mat_fn_common_plus
 * @return 生成された式。NULLは返らない。
 */
mat_expr_t* mat_fn_common_times(mat_expr_t*, mat_expr_t*);
/**
 * Divide[A, B] 式を作成する。
 * @see mat_fn_common_plus
 * @return 生成された式。NULLは返らない。
 */
mat_expr_t* mat_fn_common_divide(mat_expr_t*, mat_expr_t*);
