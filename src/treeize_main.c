#include "config.inc.h"

#include <gmp.h>
#include <stdio.h>
#include "expr.h"
#include "parser.h"
#include "world.h"
#include "fns/fns.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <expression>\n", argv[0]);
		return 1;
	}
	const char* expr = argv[1];
//	printf("expression:\t%s\n", expr);

	int exitcode;
	mat_world_t* world = mat_world_new();
	mat_fn_put_stdfunc(world);

	mat_parser_t* parser = mat_parser_new(world, expr);
	mat_expr_t* e = mat_parser_parse(parser);
	if (!e) {
		mat_parser_describe_error_position(parser, "<input>");
		exitcode = 1;
		goto free_parser;
	}

//	e->op_def->show_expression(e);
//	printf("\n");

	mpq_t result;
	mpq_init(result);
	if (mat_op_calc_value(world, e, result)) {
		exitcode = 1;
		goto free_result;
	}
	printf("%f\n", mpq_get_d(result));
	exitcode = 0;

free_result:
	mpq_clear(result);
	mat_expr_free(e);
free_parser:
	mat_parser_free(parser);
	mat_world_free(world);
	return exitcode;
}
