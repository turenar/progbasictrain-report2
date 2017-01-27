#include "config.inc.h"

#include "fn_common.h"
#include "assert.h"
#include <stdio.h>
#include "../expr.h"
#include "../op.h"

void mat_fn_common_show_expression(const mat_expr_t* expr) {
	assert(expr->op_id >= 0);

	printf("%s[", expr->op_def->name);
	mat_op_expr_t op_expr = expr->value.expr;
	for (unsigned int i = 0; i < op_expr.count; ++i) {
		if (i != 0) {
			printf(", ");
		}
		mat_expr_t* arg = op_expr.args[i];
		arg->op_def->show_expression(arg);
	}
	printf("]");
}
