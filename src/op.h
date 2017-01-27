#pragma once

#include "fwddecl.h"
#include <gmp.h>

struct mat_op_def {
	const char* name;
	void (* show_expression)(mat_expr_t*);
	mpq_t (* calc_value)(mat_expr_t*);
};

typedef struct mat_op_def mat_op_def_t;
