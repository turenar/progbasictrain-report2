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


mat_tokenizer_t* mat_tokenizer_new(const char*);
void mat_tokenizer_free(mat_tokenizer_t*);

mat_tokenizer_token_type_t mat_tokenizer_next(mat_tokenizer_t*);
const char* mat_tokenizer_get_token(mat_tokenizer_t*);
char* mat_tokenizer_dup_token(mat_tokenizer_t*) __attribute_malloc__;

size_t mat_tokenizer_get_row(mat_tokenizer_t*);
size_t mat_tokenizer_get_col(mat_tokenizer_t*);

const char* mat_tokenizer_get_token_type_name(mat_tokenizer_token_type_t);
