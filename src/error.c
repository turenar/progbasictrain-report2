#include "config.inc.h"

#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


static const char* default_error_message[MAT_ERROR_MAX] = {
		"success", // MAT_SUCCESS
		"failure", // MAT_FAILURE
		"allocation failed", // MAT_ALLOCATION_FAILURE
		"tokenizer error", // MAT_TOKENIZER_ERROR
		"unexpected token", // MAT_PARSER_UNEXPECTED_TOKEN
		"unknown func", // MAT_UNKNOWN_FUNC
		"arguments count error", // MAT_ARG_COUNT_MISMATCH
		"expression has unresolved variable", // MAT_HAVE_VARIABLE
		"divided by zero", // MAT_DIVIDED_BY_ZERO
		"arithmetic error" // MAT_ARITHMETIC_ERROR
		//MAT_ERROR_MAX
};

static char* format_with_allocation(const char* fmt, va_list ap);

void mat_err_init(mat_error_info_t* e) {
	e->error_id = MAT_SUCCESS;
	e->error_msg = NULL;
}

mat_error_t mat_err_set(mat_error_info_t* e, mat_error_t errid) {
	return mat_err_set_format(e, errid, NULL);
}

mat_error_t mat_err_set_format(mat_error_info_t* e, mat_error_t errid, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char* msg = !fmt ? NULL : format_with_allocation(fmt, ap);
	va_end(ap);
	if (e->error_msg) {
		free(e->error_msg);
	}
	e->error_id = errid;
	e->error_msg = msg;
	return errid;
}

void mat_err_clear(mat_error_info_t* e) {
	free(e->error_msg);
	e->error_msg = NULL;
	e->error_id = MAT_SUCCESS;
}

const char* mat_err_get(mat_error_info_t* e) {
	if (e->error_id == MAT_SUCCESS) {
		return NULL;
	} else if (e->error_msg == NULL) {
		if (e->error_id >= MAT_ERROR_MAX) {
			return "unknown error";
		} else {
			return default_error_message[e->error_id];
		}
	} else {
		return e->error_msg;
	}
}

mat_error_t mat_err_get_id(mat_error_info_t* e) {
	return e->error_id;
}

static char* format_with_allocation(const char* fmt, va_list ap) {
	int n;
	int size = 100;     /* Guess we need no more than 100 bytes */
	char* p;

	p = (char*) malloc((size_t) size);
	if (p == NULL) {
		return NULL;
	}

	while (true) {
		/* Try to print in the allocated space */
		va_list args;
		va_copy(args, ap);
		n = vsnprintf(p, (size_t) size, fmt, args);
		va_end(args);
		/* Check error code */
		if (n < 0) {
			free(p);
			return NULL;
		}

		/* If that worked, return the string */
		if (n < size) {
			return p;
		}

		/* Else try again with more space */
		size = n + 1;       /* Precisely what is needed */

		char* np = (char*) realloc(p, (size_t) size);
		if (np == NULL) {
			free(p);
			return NULL;
		} else {
			p = np;
		}
	}
}
