#pragma once

#include <stdbool.h>
#include "config.inc.h"

typedef enum mat_error {
	MAT_SUCCESS,
	MAT_FAILURE,
	MAT_ALLOCATION_FAILURE,
	MAT_TOKENIZER_ERROR,
	MAT_PARSER_UNEXPECTED_TOKEN,
	MAT_UNKNOWN_FUNC,
	MAT_ARG_COUNT_MISMATCH,
	MAT_HAVE_VARIABLE,
	MAT_DIVIDED_BY_ZERO,
	MAT_ARITHMETIC_ERROR,
	MAT_ERROR_MAX
} mat_error_t;

typedef struct mat_error_info {
	mat_error_t error_id;
	char* error_msg;
} mat_error_info_t;

void mat_err_init(mat_error_info_t*);
mat_error_t mat_err_set(mat_error_info_t*, mat_error_t);
mat_error_t mat_err_set_format(mat_error_info_t*, mat_error_t, const char*, ...) __attribute_format__(printf, 3, 4);
void mat_err_clear(mat_error_info_t*);
const char* mat_err_get(mat_error_info_t*);
mat_error_t mat_err_get_id(mat_error_info_t*);
