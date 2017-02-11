#pragma once

#include <gmp.h>
#include "error.h"
#include "fwddecl.h"

struct mat_op_def {
	const char* name;
	unsigned short min_args;
	unsigned short max_args;
	void (* show_expression)(const mat_expr_t*);
	mat_error_t (* calc_value)(const mat_expr_t*, mpq_t);
	mat_expr_t* (* make_differential)(const mat_expr_t*);
};

typedef struct mat_op_def mat_op_def_t;

void mat_op_show_expression(const mat_expr_t*);
mat_error_t mat_op_calc_value(const mat_expr_t*, mpq_t);
mat_expr_t* mat_op_make_differential(const mat_expr_t*);
