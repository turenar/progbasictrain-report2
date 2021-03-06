#include "config.inc.h"

#include "expr.h"
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "fns/fns.h"

mat_expr_t* mat_expr_new_const(const mpq_t constant) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op_type = MAT_OP_CONSTANT;
	expr->op_def = &mat_fn_const;
	mpq_init(expr->value.constant);
	mpq_set(expr->value.constant, constant);
	return expr;
}

mat_expr_t* mat_expr_new_const_int(int i) {
	mpq_t r;
	mpq_init(r);
	mpq_set_si(r, i, 1);
	mat_expr_t* ret = mat_expr_new_const(r);
	mpq_clear(r);
	return ret;
}

mat_expr_t* mat_expr_new_var(mat_variable_name_t c) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op_type = MAT_OP_VARIABLE;
	expr->op_def = &mat_fn_variable;
	expr->value.var = c;
	return expr;
}

mat_expr_t* mat_expr_new_args(const mat_op_def_t* op_def, unsigned int count, mat_expr_t** args_orig) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	mat_expr_t** args = (mat_expr_t**) malloc(sizeof(mat_expr_t*) * count);
	memcpy(args, args_orig, sizeof(mat_expr_t*) * count);
	expr->op_type = MAT_OP_FUNCTION;
	expr->value.func.count = count;
	expr->value.func.args = args;
	expr->op_def = op_def;
	return expr;
}

mat_expr_t* mat_expr_new_uni_arg(const mat_op_def_t* op_def, mat_expr_t* args_orig) {
	return mat_expr_new_args(op_def, 1, &args_orig);
}

mat_expr_t* mat_expr_new_bi_args(const mat_op_def_t* op_def, mat_expr_t* a, mat_expr_t* b) {
	mat_expr_t* args[] = {a, b};
	return mat_expr_new_args(op_def, 2, args);
}

mat_expr_t* mat_expr_new_from(const mat_expr_t* orig) {
	mat_expr_t* ret = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	ret->op_type = orig->op_type;
	ret->op_def = orig->op_def;
	if (mat_expr_is_const(orig)) {
		mpq_init(ret->value.constant);
		mpq_set(ret->value.constant, orig->value.constant);
	} else if (mat_expr_is_variable(orig)) {
		ret->value.var = orig->value.var;
	} else {
		assert(mat_expr_is_function(orig));
		mat_op_expr_t expr = orig->value.func;
		unsigned int count = expr.count;
		mat_expr_t** args = (mat_expr_t**) malloc(sizeof(mat_expr_t*) * count);
		for (unsigned int i = 0; i < count; ++i) {
			args[i] = mat_expr_new_from(expr.args[i]);
		}
		ret->value.func.count = count;
		ret->value.func.args = args;
	}
	return ret;
}


void mat_expr_free(mat_expr_t* expr) {
	if (!expr) {
		return;
	}
	if (mat_expr_is_const(expr)) {
		mpq_clear(expr->value.constant);
	} else if (mat_expr_is_function(expr)) {
		struct mat_expr** args = expr->value.func.args;
		for (unsigned int i = 0; i < expr->value.func.count; ++i) {
			mat_expr_free(args[i]);
		}
		free(args);
	}
	free(expr);
}

bool mat_expr_is_const(const mat_expr_t* expr) {
	return expr->op_type == MAT_OP_CONSTANT;
}

bool mat_expr_is_variable(const mat_expr_t* expr) {
	return expr->op_type == MAT_OP_VARIABLE;
}

bool mat_expr_is_function(const mat_expr_t* expr) {
	return expr->op_type == MAT_OP_FUNCTION;
}
