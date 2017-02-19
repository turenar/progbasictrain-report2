#pragma once

#include "fwddecl.h"

mat_world_t* mat_world_new() __attribute_malloc__;
void mat_world_free(mat_world_t*);

/**
 * 関数を登録する。パーサで使用するためには登録する必要がある
 * @param name 関数名
 */
void mat_world_put_op(mat_world_t*, const char* name, const mat_op_def_t*);
/**
 * @return 未登録の場合NULL
 */
const mat_op_def_t* mat_world_get_op(const mat_world_t*, const char* name);

/**
 * 変数を登録する
 * @param variable_name 変数名
 * @param x 式。定数式である必要はないが、循環参照にならないようにするべき。
 */
void mat_world_put_variable(mat_world_t*, mat_variable_name_t variable_name, mat_expr_t* x);
/**
 * @return 登録されていなければNULL
 */
mat_expr_t* mat_world_get_variable(mat_world_t*, mat_variable_name_t variable_name);

/**
 * マルチスレッド用にエラー情報もmat_world_tが持つ仕様になっている
 * @return
 */
mat_error_info_t* mat_world_get_error_info(mat_world_t*);
