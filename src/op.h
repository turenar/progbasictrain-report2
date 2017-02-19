#pragma once

#include "error.h"
#include "fwddecl.h"
#include "compat/gmp_wrapper.h"

/**
 * 関数定義。全ての値は有効な値でなければならない
 */
struct mat_op_def {
	/** 関数の名前 */
	const char* name;
	/** 取りうる最小の引数の数 */
	unsigned short min_args;
	/** 取りうる最大の引数の数 */
	unsigned short max_args;
	/**
	 * 式を表示する関数
	 */
	void (* show_expression)(mat_world_t*, const mat_expr_t*);
	/**
	 * 値を計算する。
	 * @param r 計算後の値。返り値がエラーの場合は不定である。
	 * @return エラーが発生したかどうか
	 */
	mat_error_t (* calc_value)(mat_world_t*, const mat_expr_t*, mpq_t r);
	/**
	 * 微分した後の式を返す。
	 * @return エラーが発生しない限り非NULL
	 */
	mat_expr_t* (* make_differential)(mat_world_t*, const mat_expr_t*);
	/**
	 * 受け取った式の簡略化式を返す。
	 *
	 * 簡略化というと分かりにくいが、明らかに冗長な式 (0+xなど) は簡略化の対象である。
	 * sin(1)やexp(1)のような定数として簡略化出来るが、sinやexpであることに意味がありそうな
	 * 式は簡略化の対象にするべきではない (簡略化レベルを指定できたほうが良かったのかもしれない)。
	 * そういう観点で言えば、sin(0)、exp(0)は簡略化対象から外しても、対象としても良いかもしれない。
	 * @return エラーが発生しない限り非NULL
	 */
	mat_expr_t* (* simplify)(mat_world_t*, const mat_expr_t*);
};

void mat_op_show_expression(mat_world_t*, const mat_expr_t*);
mat_error_t mat_op_calc_value(mat_world_t*, const mat_expr_t*, mpq_t);
mat_expr_t* mat_op_make_differential(mat_world_t*, const mat_expr_t*);
mat_expr_t* mat_op_simplify(mat_world_t*, const mat_expr_t*);
