#pragma once

#include "fwddecl.h"

/**
 * str->mat_op_def_tハッシュマップインスタンスを作成する
 * @return allocateされたインスタンス
 */
mat_op_hash_t* mat_op_hash_new();
/**
 * mat_op_def_t を登録する
 * @param name 関数名
 */
void mat_op_hash_put(mat_op_hash_t*, const char* name, const mat_op_def_t*);
/**
 * @return 指定した名前の mat_op_def_t。登録されていない場合はNULL。
 */
const mat_op_def_t* mat_op_hash_get(mat_op_hash_t*, const char*);
/**
 * 解放
 */
void mat_op_hash_free(mat_op_hash_t*);
