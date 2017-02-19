#pragma once

#include <stdbool.h>
#include "fwddecl.h"
#include "compat/gmp_wrapper.h"

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
	mat_variable_name_t var;
	mpq_t constant;
	mat_op_expr_t expr;
} mat_value_t;

struct mat_expr {
	mat_op_id_t op_type;
	const mat_op_def_t* op_def;
	mat_value_t value;
};


mat_expr_t* mat_expr_new_const(const mpq_t);
mat_expr_t* mat_expr_new_const_int(int);
mat_expr_t* mat_expr_new_var(mat_variable_name_t);
mat_expr_t* mat_expr_new_args(const mat_op_def_t*, unsigned int count, mat_expr_t**);
mat_expr_t* mat_expr_new_uni_arg(const mat_op_def_t* op_def, mat_expr_t* args_orig);
mat_expr_t* mat_expr_new_bi_args(const mat_op_def_t* op_def, mat_expr_t* a, mat_expr_t* b);
mat_expr_t* mat_expr_new_from(const mat_expr_t*);

void mat_expr_free(mat_expr_t*);

bool mat_expr_is_const(const mat_expr_t* expr);
bool mat_expr_is_variable(const mat_expr_t* expr);
bool mat_expr_is_function(const mat_expr_t* expr);
