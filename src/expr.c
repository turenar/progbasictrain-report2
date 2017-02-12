#include "config.inc.h"

#include "expr.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "fns/fns.h"

mat_expr_t* mat_expr_new_const(mpq_t constant) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op_id = MAT_OP_CONSTANT;
	expr->op_def = &mat_fn_const;
	mpq_init(expr->value.constant);
	mpq_set(expr->value.constant, constant);
	return expr;
}

mat_expr_t* mat_expr_new_const_double(double d) {
	mpq_t r;
	mpq_init(r);
	mpq_set_d(r, d);
	mat_expr_t* ret = mat_expr_new_const(r);
	mpq_clear(r);
	return ret;
}

mat_expr_t* mat_expr_new_var(char c) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op_id = MAT_OP_VARIABLE;
	expr->op_def = &mat_fn_variable;
	expr->value.var = c;
	return expr;
}

mat_expr_t* mat_expr_new_args(const mat_op_def_t* op_def, unsigned int count, mat_expr_t** args_orig) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	mat_expr_t** args = (mat_expr_t**) malloc(sizeof(mat_expr_t*) * count);
	memcpy(args, args_orig, sizeof(mat_expr_t*) * count);
	expr->op_id = MAT_OP_FUNCTION;
	expr->value.expr.count = count;
	expr->value.expr.args = args;
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
	ret->op_id = orig->op_id;
	ret->op_def = orig->op_def;
	if (orig->op_id == MAT_OP_CONSTANT) {
		mpq_init(ret->value.constant);
		mpq_set(ret->value.constant, orig->value.constant);
	} else if (orig->op_id == MAT_OP_VARIABLE) {
		ret->value.var = orig->value.var;
	} else if (orig->op_id == MAT_OP_FUNCTION) {
		mat_op_expr_t expr = orig->value.expr;
		unsigned int count = expr.count;
		mat_expr_t** args = (mat_expr_t**) malloc(sizeof(mat_expr_t*) * count);
		for (unsigned int i = 0; i < count; ++i) {
			args[i] = mat_expr_new_from(expr.args[i]);
		}
		ret->value.expr.count = count;
		ret->value.expr.args = args;
	}
	return ret;
}


void mat_expr_free(mat_expr_t* expr) {
	if (expr->op_id == MAT_OP_CONSTANT || expr->op_id == 0) {
		mpq_clear(expr->value.constant);
	} else if (expr->op_id == MAT_OP_FUNCTION) {
		struct mat_expr** args = expr->value.expr.args;
		for (unsigned int i = 0; i < expr->value.expr.count; ++i) {
			mat_expr_free(args[i]);
		}
		free(args);
	}
	free(expr);
}

bool mat_expr_is_const(const mat_expr_t* expr) {
	return expr->op_id == MAT_OP_CONSTANT;
}

bool mat_expr_is_variable(const mat_expr_t* expr) {
	return expr->op_id == MAT_OP_VARIABLE;
}

bool mat_expr_is_function(const mat_expr_t* expr) {
	return expr->op_id == MAT_OP_FUNCTION;
}
