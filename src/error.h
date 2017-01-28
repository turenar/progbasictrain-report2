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
	MAT_HAVE_VARIABLE
} mat_error_t;

void mat_err_set_format(mat_error_t, const char*, ...) __attribute_format__(printf, 2, 3);
void mat_err_clear();
const char* mat_err_get();
mat_error_t mat_err_get_id();
