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
		mat_parser_describe_error_position(parser);
		exitcode = 1;
		goto free_parser;
	}

	mat_expr_t* differential;
	mat_expr_t* simplified;
	differential = mat_op_make_differential(world, e);
	if (!differential) {
		exitcode = 1;
		goto free_parser;
	}
	simplified = mat_op_simplify(world, differential);
	if (!simplified) {
		exitcode = 1;
		goto free_diff;
	}
	mat_op_show_expression(world, simplified);
	printf("\n");

	exitcode = 0;

	mat_expr_free(simplified);
free_diff:
	mat_expr_free(differential);
	mat_expr_free(e);
free_parser:
	mat_parser_free(parser);
	mat_world_free(world);
	return exitcode;
}
