#include <gmp.h>
#include <stdbool.h>
#include <stdio.h>
#include "error.h"
#include "expr.h"
#include "op.h"
#include "parser.h"
#include "world.h"

int main() {
	const char* expr = "Log[UnknownUUUDFAsdjfkjsdflkajsdlfkjasdlfkjasdjhsdkfjhaskdfjhaskdjfhaskdjfhaskdfjhasfhaksdjfhaksdjfhaksdjfhaks[2]]";
	printf("expression:\t%s\n", expr);

	int exitcode;
	mat_world_t* world = mat_world_new();
	mat_world_put_stdfunc(world);

	mat_parser_t* parser = mat_parser_new(world, expr);
	mat_expr_t* e = mat_parser_parse(parser);
	if (!e) {
		mat_parser_describe_error_position(parser);
		exitcode = 1;
		goto free_parser;
	}

	e->op_def->show_expression(e);
	printf("\n");

	mpq_t result = {0};
	mpq_init(result);
	if (e->op_def->calc_value(e, result)) {
		exitcode = 1;
		goto free_expr;
	}
	printf("result: %f\n", mpq_get_d(result));
	mpq_clear(result);

	exitcode = 0;

free_expr:
	mat_expr_free(e);
free_parser:
	mat_parser_free(parser);
	mat_world_free(world);
	return exitcode;
}
