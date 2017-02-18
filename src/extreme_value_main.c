#include "config.inc.h"

#include <stdio.h>
#include "expr.h"
#include "parser.h"
#include "newton.h"
#include "world.h"
#include "fns/fns.h"

int main(int argc, char** argv) {
	int exitcode;

	mat_world_t* world = mat_world_new();
	mat_fn_put_stdfunc(world);

	const char* program_name = argv[0];

	if (argc != 2) {
		fprintf(stderr, "%s: missing expression\n", program_name);
		exitcode = 1;
		goto free_world;
	}

	mat_parser_t* parser;
	mat_expr_t* e;
	parser = mat_parser_new(world, argv[1]);
	e = mat_parser_parse(parser);
	if (!e) {
		mat_parser_describe_error_position(parser, "<input>");
		exitcode = 1;
		goto free_parser;
	}

	mat_expr_t* differential;
	differential = mat_op_make_differential(world, e);
	if (!differential) {
		exitcode = 1;
		goto free_parser;
	}


	mpq_t initial, epsilon, result;
	mpq_inits(initial, epsilon, result, NULL);
	mpq_set_ui(initial, 1, 1);
	mpq_set_ui(epsilon, 1, 10000000);
	if (mat_newton_optimize(world, differential, initial, epsilon, result)) {
		exitcode = 1;
		goto free_result;
	}
	printf("%f\n", mpq_get_d(result));
	exitcode = 0;

free_result:
	mpq_clears(initial, epsilon, result, NULL);
	mat_expr_free(differential);
	mat_expr_free(e);
free_parser:
	mat_parser_free(parser);
free_world:
	mat_world_free(world);
	return exitcode;
}
