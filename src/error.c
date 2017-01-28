#include "config.inc.h"

#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


static mat_error_t error_id = MAT_SUCCESS;
static char* error_msg = NULL;

static char* format_with_allocation(const char* fmt, va_list ap);

void mat_err_set_format(mat_error_t errid, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char* msg = format_with_allocation(fmt, ap);
	va_end(ap);
	if (error_msg) {
		free(error_msg);
	}
	if (msg) {
		error_id = errid;
		error_msg = msg;
	} else {
		error_id = MAT_FAILURE;
		error_msg = NULL;
	}
}

void mat_err_clear() {
	error_msg = NULL;
	error_id = MAT_SUCCESS;
}

const char* mat_err_get() {
	if (error_id == MAT_SUCCESS) {
		return NULL;
	} else if (error_msg == NULL) {
		return "unknown error";
	} else {
		return error_msg;
	}
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
