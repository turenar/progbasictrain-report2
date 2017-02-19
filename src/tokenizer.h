#pragma once

#include <stddef.h>
#include "config.inc.h"
#include "fwddecl.h"

typedef enum mat_tokenizer_token_type {
	MAT_TOKEN_INITIAL,
	MAT_TOKEN_UNKNOWN,
	MAT_TOKEN_END_OF_EXPRESSION,
	MAT_TOKEN_FUNC_NAME,
	MAT_TOKEN_FUNC_OPENING_BRACKET,
	MAT_TOKEN_FUNC_ARG_SEPARATOR,
	MAT_TOKEN_FUNC_CLOSING_BRACKET,
	MAT_TOKEN_VARIABLE,
	MAT_TOKEN_LITERAL,
	MAT_TOKEN_TOKEN_TYPE_MAX
} mat_tokenizer_token_type_t;

extern const char* mat_tokenizer_name[MAT_TOKEN_TOKEN_TYPE_MAX];


/**
 * @param expr_str トークナイズする文字列。当然だがトークナイズ中に文字列の内容が変化した場合の挙動は不定である。
 * @return インスタンス
 */
mat_tokenizer_t* mat_tokenizer_new(const char* expr_str);
void mat_tokenizer_free(mat_tokenizer_t*);

/**
 * トークンを次に進め取得する
 * @return トークンタイプ
 */
mat_tokenizer_token_type_t mat_tokenizer_next(mat_tokenizer_t*);
/**
 * 現在のトークン文字列を取得する。内部バッファに対するポインタを返し、現在位置が移動した後の文字列の内容は不定である。
 * @return トークン文字列。変更不可。
 */
const char* mat_tokenizer_get_token(mat_tokenizer_t*);
/**
 * 現在のトークン文字列のコピーを取得する。mallocされた文字列なので自由に変更してよいが、freeを忘れないように。
 * @return トークン文字列のコピー
 */
char* mat_tokenizer_dup_token(mat_tokenizer_t*) __attribute_malloc__;

/**
 * @return トークナイズしている文字列 (全部)
 */
const char* mat_tokenizer_get_str(mat_tokenizer_t*);
/**
 * @return 現在位置を含む行の始点。終端はトークナイズしている文字列の終端と一致する。
 */
const char* mat_tokenizer_get_row_str(mat_tokenizer_t*);
/**
 * @return 現在位置の行番号 (1start)
 */
size_t mat_tokenizer_get_row(mat_tokenizer_t*);
/**
 * @return 現在位置の列番号 (1start)
 */
size_t mat_tokenizer_get_col(mat_tokenizer_t*);
/**
 * @return 現在のトークン文字列の長さ
 */
size_t mat_tokenizer_get_token_len(mat_tokenizer_t*);

/**
 * ユーティリティー関数
 * @return トークンの種別名
 */
const char* mat_tokenizer_get_token_type_name(mat_tokenizer_token_type_t);
