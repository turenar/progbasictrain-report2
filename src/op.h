#pragma once

#include "expr.h"

typedef struct mat_op_def {
	mpq_t (* calc_value)(mat_expr_t);
};
