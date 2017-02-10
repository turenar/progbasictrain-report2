#include "config.inc.h"

#include "expr.h"
#include <stdlib.h>
#include <string.h>
#include "fns/fns.h"

const mat_op_id_t MAT_OP_CONSTANT = -1;
const mat_op_id_t MAT_OP_VARIABLE = -2;

mat_expr_t* mat_expr_new_const(mpq_t constant) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op_id = MAT_OP_CONSTANT;
	expr->op_def = &mat_fn_const;
	mpq_init(expr->value.constant);
	mpq_set(expr->value.constant, constant);
	return expr;
}

mat_expr_t* mat_expr_new_var(char c) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op_id = MAT_OP_VARIABLE;
	expr->op_def = &mat_fn_variable;
	expr->value.var = c;
	return expr;
}

mat_expr_t* mat_expr_new_args(mat_op_id_t op, const mat_op_def_t* op_def, unsigned int count, mat_expr_t** args_orig) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	mat_expr_t** args = (mat_expr_t**) malloc(sizeof(mat_expr_t*) * count);
	memcpy(args, args_orig, sizeof(mat_expr_t*) * count);
	expr->op_id = op;
	expr->value.expr.count = count;
	expr->value.expr.args = args;
	expr->op_def = op_def;
	return expr;
}

void mat_expr_free(mat_expr_t* expr) {
	if (expr->op_id == MAT_OP_CONSTANT) {
		mpq_clear(expr->value.constant);
	} else if (expr->op_id >= 0) {
		struct mat_expr** args = expr->value.expr.args;
		for (unsigned int i = 0; i < expr->value.expr.count; ++i) {
			mat_expr_free(args[i]);
		}
		free(args);
	}
	free(expr);
}
