#pragma once

#include <stdbool.h>
#include "fwddecl.h"
#include "compat/gmp_wrapper.h"

typedef enum mat_op_id {
	/** 定数。 mat_value_t.constant が使用できる */
			MAT_OP_CONSTANT,
	/** 変数。 mat_value_t.var が使用できる */
			MAT_OP_VARIABLE,
	/** 関数。 mat_value_t.func が使用できる */
			MAT_OP_FUNCTION
} mat_op_id_t;

/**
 * 関数の動作に必要な引数とその数が格納される。基本的に mat_expr_new_args を使用すること。
 */
typedef struct mat_op_expr {
	unsigned int count;
	/**
	 * mallocされたポインタ (NULL不可)。要素もNULL不可。count個分は正しい mat_expr_t である必要がある。
	 */
	mat_expr_t** args;
} mat_op_expr_t;

typedef union {
	/** mat_expr_is_variable が真である場合に使用できる。変数名 (1文字) が入る。 */
	mat_variable_name_t var;
	/** mat_expr_is_const が真である場合に使用できる。initされた定数が入る。 */
	mpq_t constant;
	/** mat_expr_is_function が真である場合に使用できる。関数の引数が入る。 */
	mat_op_expr_t func;
} mat_value_t;

/**
 * 式の木構造のノード。
 */
struct mat_expr {
	/** 式のタイプ。直接定数と比較するより、ユーティリティー関数を使用すべき */
	mat_op_id_t op_type;
	/** 式の定義へのポインタ。 mat_expr_free では解放されない。 */
	const mat_op_def_t* op_def;
	/** 式の動作に必要な値などが入る */
	mat_value_t value;
};


/**
 * mpq_t を使用して式を作成する
 * @return allocateされた式
 */
mat_expr_t* mat_expr_new_const(const mpq_t);
/**
 * int を使用して式を作成する
 * @return allocateされた式
 */
mat_expr_t* mat_expr_new_const_int(int);
/**
 * 変数式を作成する
 * @return allocateされた式
 */
mat_expr_t* mat_expr_new_var(mat_variable_name_t);
/**
 * 渡された引数を使用して関数式を作成する
 * @param count 引数の数
 * @param args 引数が入った配列 (count個の有効な式が入っている必要がある)。要素の所有権は全て明け渡す。
 * @return allocateされた式
 */
mat_expr_t* mat_expr_new_args(const mat_op_def_t*, unsigned int count, mat_expr_t** args);
/**
 * 一つの引数を持つ関数式を作成するユーティリティー関数。
 * @param op_def 関数定義
 * @param args_orig 引数。所有権が移動する。
 * @return allocateされた式
 */
mat_expr_t* mat_expr_new_uni_arg(const mat_op_def_t* op_def, mat_expr_t* args_orig);
/**
 * 二つの引数を持つ関数式を作成するユーティリティー関数。
 * @param op_def 関数定義
 * @param a 引数。所有権が移動する。
 * @param b 引数。所有権が移動する。
 * @return allocateされた式
 */
mat_expr_t* mat_expr_new_bi_args(const mat_op_def_t* op_def, mat_expr_t* a, mat_expr_t* b);
/**
 * すでにある式をコピーして (同等の) 新しい式を作成する。
 * @return allocateされた式
 */
mat_expr_t* mat_expr_new_from(const mat_expr_t*);

/**
 * 式を再帰的に解放する
 */
void mat_expr_free(mat_expr_t*);

/** @return 定数式であるかどうか */
bool mat_expr_is_const(const mat_expr_t*);
/** @return 変数式であるかどうか (変数式には変数を含む関数式を含まない) */
bool mat_expr_is_variable(const mat_expr_t*);
/** @return 関数式であるかどうか */
bool mat_expr_is_function(const mat_expr_t*);
