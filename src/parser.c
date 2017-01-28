#include "config.inc.h"

#include "parser.h"
#include <stdlib.h>
#include "error.h"
#include "expr.h"
#include "tokenizer.h"
#include "world.h"
#include "fns/fns.h"
#include <stdio.h>
#include <string.h>

struct mat_parser {
	mat_tokenizer_t* tokenizer;
	mat_world_t* world;
};
typedef struct mat_parser mat_parser_t;

static const size_t SHOW_ROW_LEN = 60u;


static mat_expr_t* parse_expr(mat_parser_t*, mat_tokenizer_token_type_t);
static mat_expr_t* parse_func(mat_parser_t*);
static mat_expr_t* parse_var(mat_parser_t*);
static mat_expr_t* parse_literal(mat_parser_t*);

static size_t get_row_len(const char*);

mat_parser_t* mat_parser_new(mat_world_t* w, const char* expr) {
	mat_parser_t* ret = (mat_parser_t*) malloc(sizeof(mat_parser_t));
	ret->world = w;
	ret->tokenizer = mat_tokenizer_new(expr);
	return ret;
}

mat_expr_t* mat_parser_parse(mat_parser_t* parser) {
	return parse_expr(parser, mat_tokenizer_next(parser->tokenizer));
}

void mat_parser_free(mat_parser_t* parser) {
	mat_tokenizer_free(parser->tokenizer);
	free(parser);
}

void mat_parser_describe_error_position(mat_parser_t* parser) {
	const char* error_msg = mat_err_get();
	size_t row_pos = mat_tokenizer_get_row(parser->tokenizer);
	size_t col_pos = mat_tokenizer_get_col(parser->tokenizer);
	printf("<input>:%zu:%zu: \033[1;31mError\033[0m: %s\n", row_pos, col_pos, error_msg);

	const char* row = mat_tokenizer_get_row_str(parser->tokenizer);
	size_t row_len = get_row_len(row);
	fwrite(row, 1u, row_len, stdout);
	putchar('\n');
	size_t snake = col_pos - 1;
	for (size_t i = 0; i < snake; ++i) {
		putchar(' ');
	}
	printf("\033[1;32m^\033[0;32m");
	snake = mat_tokenizer_get_token_len(parser->tokenizer) - 1;
	for (size_t i = 0; i < snake; ++i) {
		putchar('~');
	}
	printf("\033[0m\n");
}


static mat_expr_t* parse_expr(mat_parser_t* parser, mat_tokenizer_token_type_t t) {
	if (t == MAT_TOKEN_FUNC_NAME) {
		return parse_func(parser);
	} else if (t == MAT_TOKEN_VARIABLE) {
		return parse_var(parser);
	} else if (t == MAT_TOKEN_LITERAL) {
		return parse_literal(parser);
	} else {
		mat_err_set_format(MAT_PARSER_UNEXPECTED_TOKEN, "unexpected token. expected=%s|%s|%s, actual=%s",
		                   mat_tokenizer_name[MAT_TOKEN_FUNC_NAME], mat_tokenizer_name[MAT_TOKEN_VARIABLE],
		                   mat_tokenizer_name[MAT_TOKEN_LITERAL], mat_tokenizer_get_token_type_name(t));
		return NULL;
	}
}

static mat_expr_t* parse_func(mat_parser_t* parser) {
	mat_tokenizer_token_type_t next_token_type;

	const char* func_name = mat_tokenizer_get_token(parser->tokenizer);
	mat_op_def_t* op_def = mat_world_get_op(parser->world, func_name);
	if (op_def == NULL) {
		mat_err_set_format(MAT_UNKNOWN_FUNC, "unknown function: %s", func_name);
		goto free_func_name;
	}

	next_token_type = mat_tokenizer_next(parser->tokenizer);
	if (next_token_type != MAT_TOKEN_FUNC_OPENING_BRACKET) {
		mat_err_set_format(MAT_PARSER_UNEXPECTED_TOKEN, "unexpected token. expected=%s, actual=%s",
		                   mat_tokenizer_name[MAT_TOKEN_FUNC_OPENING_BRACKET],
		                   mat_tokenizer_get_token_type_name(next_token_type));
		goto free_func_name;
	}

	unsigned int arg_count;
	mat_expr_t* args[30];
	arg_count = 0;

	next_token_type = mat_tokenizer_next(parser->tokenizer);
	if (next_token_type != MAT_TOKEN_FUNC_CLOSING_BRACKET) {
		while (true) {
			mat_expr_t* arg = parse_expr(parser, next_token_type);
			if (arg == NULL) {
				goto free_args;
			}
			args[arg_count++] = arg;

			next_token_type = mat_tokenizer_next(parser->tokenizer);
			if (next_token_type == MAT_TOKEN_FUNC_CLOSING_BRACKET) {
				break;
			} else if (next_token_type == MAT_TOKEN_FUNC_ARG_SEPARATOR) {
				next_token_type = mat_tokenizer_next(parser->tokenizer);
			} else {
				mat_err_set_format(MAT_PARSER_UNEXPECTED_TOKEN, "unexpected token. expected=%s|%s, actual=%s",
				                   mat_tokenizer_name[MAT_TOKEN_FUNC_CLOSING_BRACKET],
				                   mat_tokenizer_name[MAT_TOKEN_FUNC_ARG_SEPARATOR],
				                   mat_tokenizer_get_token_type_name(next_token_type));
				goto free_args;
			}
		}
	}

	if (arg_count < op_def->min_args || op_def->max_args < arg_count) {
		if (op_def->min_args == op_def->max_args) {
			mat_err_set_format(MAT_ARG_COUNT_MISMATCH, "%s takes %u arguments, but %u were given",
			                   op_def->name, op_def->min_args, arg_count);
		} else {
			mat_err_set_format(MAT_ARG_COUNT_MISMATCH, "%s takes %u-%u arguments, but %u were given",
			                   op_def->name, op_def->min_args, op_def->max_args, arg_count);
		}
		goto free_args;
	}

	return mat_expr_new_args(1, op_def, arg_count, args);

free_args:
	for (unsigned int i = 0; i < arg_count; ++i) {
		mat_expr_free(args[i]);
	}
free_func_name:
	// not alloced
	return NULL;
}

static mat_expr_t* parse_var(mat_parser_t* parser) {
	const char* buf = mat_tokenizer_get_token(parser->tokenizer);
	return mat_expr_new_var(buf[0]);
}

static mat_expr_t* parse_literal(mat_parser_t* parser) {
	const char* buf = mat_tokenizer_get_token(parser->tokenizer);
	mpf_t st;
	mpf_init(st);
	mpf_set_str(st, buf, 10);
	mpq_t r;
	mpq_init(r);
	mpq_set_f(r, st);
	mat_expr_t* expr = mat_expr_new_const(r);
	mpf_clear(st);
	mpq_clear(r);
	return expr;
}

static size_t get_row_len(const char* str) {
	size_t l = 0;
	for (const char* p = str; *p != '\0'; ++p) {
		if (*p == '\r' || *p == '\n') {
			break;
		}
		++l;
	}
	return l;
}
