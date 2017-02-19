#pragma once

typedef unsigned char mat_variable_name_t;

struct mat_parser;
/**
 * パーサー
 */
typedef struct mat_parser mat_parser_t;

struct mat_expr;
/**
 * 式
 */
typedef struct mat_expr mat_expr_t;

struct mat_tokenizer;
/**
 * ノーステート (?) トークナイザー。現在位置以外はステートを持たず、次に得られるトークンタイプは前のトークンによらない。
 *
 * トークン位置の管理に使われている。エラーメッセージはこのトークナイザーの現在位置保持によるところが大きい。
 */
typedef struct mat_tokenizer mat_tokenizer_t;

struct mat_op_def;
typedef struct mat_op_def mat_op_def_t;

struct mat_op_hash;
typedef struct mat_op_hash mat_op_hash_t;

struct mat_world;
typedef struct mat_world mat_world_t;

struct mat_error_info;
typedef struct mat_error_info mat_error_info_t;
