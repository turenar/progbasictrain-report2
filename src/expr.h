#pragma once

#include <gmp.h>
#include "fwddecl.h"

typedef int mat_op_id_t;

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


extern const mat_op_id_t MAT_OP_CONSTANT;
extern const mat_op_id_t MAT_OP_VARIABLE;


mat_expr_t* mat_expr_new_const(mpq_t);
mat_expr_t* mat_expr_new_var(char);
mat_expr_t* mat_expr_new_args(mat_op_id_t, const mat_op_def_t*, unsigned int count, mat_expr_t**);

void mat_expr_free(mat_expr_t*);
