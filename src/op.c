#include "config.inc.h"

#include "op.h"
#include "expr.h"

void mat_op_show_expression(mat_world_t* w, const mat_expr_t* x) {
	x->op_def->show_expression(w, x);
}

mat_error_t mat_op_calc_value(mat_world_t* w, const mat_expr_t* x, mpq_t r) {
	return x->op_def->calc_value(w, x, r);
}

mat_expr_t* mat_op_make_differential(mat_world_t* w, const mat_expr_t* x) {
	return x->op_def->make_differential(w, x);
}

mat_expr_t* mat_op_simplify(mat_world_t* w, const mat_expr_t* x) {
	return x->op_def->simplify(w, x);
}
