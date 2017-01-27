#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include "parser.h"
#include "expr.h"
#include "error.h"
#include "world.h"
#include "op.h"

int main() {
	const char* expr = "Plus [ Times [ Sin [ 13.4 ] , 3 ] , 2 ]";
	printf("expression:\t%s\n", expr);

	mat_world_t* world = mat_world_new();
	mat_parser_t* parser = mat_parser_new(world, expr);
	mat_expr_t* e = mat_parser_parse(parser);
	if (!e) {
		printf("error: %s\n", mat_err_get());
		mat_parser_free(parser);
		return 1;
	}

	e->op_def->show_expression(e);
	printf("\n");

	mpq_t result = {0};
	mpq_init(result);
	e->op_def->calc_value(e, result);
	printf("result: %f\n", mpq_get_d(result));

	mat_expr_free(e);
	mat_parser_free(parser);
}
