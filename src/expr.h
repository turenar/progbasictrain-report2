#pragma once

#include <gmp.h>
#include "fwddecl.h"

typedef enum mat_op_id {
	MAT_OP_CONSTANT,
	MAT_OP_VARIABLE,
	MAT_OP_FUNCTION
} mat_op_id_t;

struct mat_expr;
typedef struct mat_expr mat_expr_t;

typedef struct mat_op_expr {
	unsigned int count;
	mat_expr_t** args;
} mat_op_expr_t;

typedef union mat_value {
	char var;
	mpq_t constant;
	mat_op_expr_t expr;
} mat_value_t;

struct mat_expr {
	mat_op_id_t op_id;
	const mat_op_def_t* op_def;
	mat_value_t value;
};


mat_expr_t* mat_expr_new_const(mpq_t);
mat_expr_t* mat_expr_new_var(char);
mat_expr_t* mat_expr_new_args(const mat_op_def_t*, unsigned int count, mat_expr_t**);

void mat_expr_free(mat_expr_t*);
