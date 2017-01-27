#include "config.inc.h"

#include "expr.h"
#include <stdlib.h>
#include <string.h>

const mat_op_id_t MAT_OP_CONSTANT = -1;
const mat_op_id_t MAT_OP_VARIABLE = -2;

mat_expr_t* mat_expr_new_const(mpq_t constant) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op = MAT_OP_CONSTANT;
	mpq_init(expr->value.constant);
	mpq_set(expr->value.constant, constant);
	return expr;
}

mat_expr_t* mat_expr_new_var(char c) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	expr->op = MAT_OP_VARIABLE;
	expr->value.var = c;
	return expr;
}

mat_expr_t* mat_expr_new_args(mat_op_id_t op, unsigned int count, mat_expr_t** args_orig) {
	mat_expr_t* expr = (mat_expr_t*) malloc(sizeof(mat_expr_t));
	mat_expr_t** args = (mat_expr_t**) malloc(sizeof(mat_expr_t*) * count);
	memcpy(args, args_orig, sizeof(mat_expr_t*) * count);
	expr->op = op;
	expr->count = count;
	expr->value.args = args;
	return expr;
}

void mat_expr_free(mat_expr_t* expr) {
	if (expr->op == MAT_OP_CONSTANT) {
		mpq_clear(expr->value.constant);
	} else if (expr->op >= 0) {
		struct mat_expr** args = expr->value.args;
		for (unsigned int i = 0; i < expr->count; ++i) {
			mat_expr_free(args[i]);
		}
		free(args);
	}
	free(expr);
}
