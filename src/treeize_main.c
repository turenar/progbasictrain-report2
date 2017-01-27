#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include "parser.h"
#include "expr.h"
#include "error.h"

static void traverse(mat_expr_t*);

int main() {
	const char* expr = "Plus [ Times [ Sin [ 13.4 ] , 3 ] , 2 ]";
	printf("expression:\t%s\n", expr);

	mat_parser_t* parser = mat_parser_new(expr);
	mat_expr_t* e = mat_parser_parse(parser);
	if (!e) {
		printf("error: %s\n", mat_err_get());
		mat_parser_free(parser);
		return 1;
	}
	traverse(e);
	mat_expr_free(e);
	mat_parser_free(parser);
}

static void traverse(mat_expr_t* expr) {
	if (expr->op == MAT_OP_CONSTANT) {
		double d = mpq_get_d(expr->value.constant);
		printf("%f", d);
	} else if (expr->op == MAT_OP_VARIABLE) {
		printf("%c", expr->value.var);
	} else {
		printf("Expr(%d)[", expr->op);
		for (unsigned int i = 0; i < expr->count; ++i) {
			if (i != 0) {
				printf(", ");
			}
			traverse(expr->value.args[i]);
		}
		printf("]");
	}
}
