#pragma once

#include "config.inc.h"

#include <stdbool.h>
#include "fwddecl.h"

typedef enum mat_error {
	MAT_SUCCESS = 0, /*!< 成功 */
	MAT_FAILURE,/*!< genericな失敗 */
	MAT_ALLOCATION_FAILURE, /*!< 確保に失敗 (使われていない) */
	MAT_PARSER_UNEXPECTED_TOKEN, /*!< パーサーの予期しないトークン*/
	MAT_UNKNOWN_FUNC, /*!< 関数名が不明 */
	MAT_ARG_COUNT_MISMATCH,/*!< 引数カウントのチェックに失敗 */
	MAT_HAVE_VARIABLE,/*!< 定義されていない変数が使用されているため値の計算に失敗 */
	MAT_DIVIDED_BY_ZERO, /*!< ゼロ除算 */
	MAT_ARITHMETIC_ERROR,/*!< (ゼロ除算以外の) 数値計算Genericエラー */
	MAT_ERROR_MAX
} mat_error_t;

struct mat_error_info {
	mat_error_t error_id;
	char* error_msg;
};

/**
 * 初期化。ゼロクリアでもいい
 */
void mat_err_init(mat_error_info_t*);
/**
 * エラーであることを設定する。
 * @param e エラー番号。MAT_SUCCESSでもいいが、その用途は mat_err_clear を使用すべき。
 * @return 引数として渡されたエラー番号をそのまま返す。
 * @see mat_err_set_format
 */
mat_error_t mat_err_set(mat_error_info_t*, mat_error_t e);
/**
 * エラーであることを設定する。
 * @param e エラー番号。MAT_SUCCESSでもいいが、その用途は mat_err_clear を使用すべき。
 * @param fmt printfとして渡せる形のフォーマット文
 * @return 引数として渡されたエラー番号をそのまま返す。
 * @see mat_err_set_format
 */
__attribute_format__(printf, 3, 4)
mat_error_t mat_err_set_format(mat_error_info_t*, mat_error_t e, const char* fmt, ...);
/**
 * エラーである状態から回復する。
 */
void mat_err_clear(mat_error_info_t*);
/**
 * エラーメッセージを取得する。エラーではあるがメッセージが指定されていない場合はデフォルトのエラーメッセージが返る。
 * @return MAT_SUCCESSの場合はNULLが返る
 */
const char* mat_err_get(mat_error_info_t*);
/**
 * @return エラー番号
 */
mat_error_t mat_err_get_id(mat_error_info_t*);
