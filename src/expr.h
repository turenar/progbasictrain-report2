#pragma once

#include <gmp.h>

typedef int mat_op_id_t;

struct mat_expr;

typedef union mat_args {
	char var;
	mpq_t constant;
	struct mat_expr** args;
} mat_value_t;

typedef struct mat_expr {
	mat_op_id_t op;
	unsigned int count;
	mat_value_t value;
} mat_expr_t;

extern const mat_op_id_t MAT_OP_CONSTANT;
extern const mat_op_id_t MAT_OP_VARIABLE;

mat_expr_t* mat_expr_new_const(mpq_t);
mat_expr_t* mat_expr_new_var(char);
mat_expr_t* mat_expr_new_args(mat_op_id_t, unsigned int count, mat_expr_t**);

void mat_expr_free(mat_expr_t*);
