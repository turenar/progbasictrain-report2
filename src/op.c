#include "config.inc.h"

#include "op.h"
#include "expr.h"

void mat_op_show_expression(const mat_expr_t* x) {
	x->op_def->show_expression(x);
}

mat_error_t mat_op_calc_value(const mat_expr_t* x, mpq_t r) {
	return x->op_def->calc_value(x, r);
}
