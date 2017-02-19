#pragma once

#include "fwddecl.h"

/**
 * @param expr 式文字列。invalid syntaxでもここではエラーにならない
 * @return 新しいインスタンス
 */
mat_parser_t* mat_parser_new(mat_world_t*, const char* expr) ;
void mat_parser_free(mat_parser_t*);

/**
 * パースされた式を返す。一度パースすると再度パースは出来ない。
 *
 * 全文字列が有効な式である必要がある。
 * @return エラーの場合NULL
 */
mat_expr_t* mat_parser_parse(mat_parser_t*);
/**
 * パースエラーとなった場所とエラーメッセージをstderrに出力する
 * @param filename ファイル名(?)
 */
void mat_parser_describe_error_position(mat_parser_t*, const char* filename);
