#include "config.inc.h"

#include "parser.h"
#include <stdlib.h>
#include "error.h"
#include "expr.h"
#include "tokenizer.h"
#include "world.h"
#include "fns/fns.h"

struct mat_parser {
	mat_tokenizer_t* tokenizer;
	mat_world_t* world;
};
typedef struct mat_parser mat_parser_t;

static mat_expr_t* parse_expr(mat_parser_t*, mat_tokenizer_token_type_t);
static mat_expr_t* parse_func(mat_parser_t*);
static mat_expr_t* parse_var(mat_parser_t*);
static mat_expr_t* parse_literal(mat_parser_t*);

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

	unsigned int arg_count = 0;
	mat_expr_t* args[30];

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
	mpf_t st = {0};
	mpf_init(st);
	mpf_set_str(st, buf, 10);
	mpq_t r = {0};
	mpq_init(r);
	mpq_set_f(r, st);
	mat_expr_t* expr = mat_expr_new_const(r);
	mpf_clear(st);
	mpq_clear(r);
	return expr;
}
