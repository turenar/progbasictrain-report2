#include "config.inc.h"

#include <getopt.h>
#include <gmp.h>
#include <stdio.h>
#include "expr.h"
#include "parser.h"
#include "world.h"
#include "fns/fns.h"

static void show_help(const char* program_name);
static mat_expr_t* parse_declare(mat_world_t*, const char* program_name, const char* expr_str);

int main(int argc, char** argv) {
	struct option longopts[] = {
			{"help",   no_argument,       NULL, 'h'},
			{"define", required_argument, NULL, 'D'},
			{NULL, 0,                     NULL, 0}
	};

	int exitcode;

	mat_world_t* world = mat_world_new();
	mat_fn_put_stdfunc(world);

	const char* program_name = argv[0];
	int opt;
	int longindex;

	while ((opt = getopt_long(argc, argv, "hD:", longopts, &longindex)) != -1) {
		switch (opt) {
			case 'h':
				show_help(program_name);
				exitcode = 0;
				goto free_world;
			case 'D': {
				mat_expr_t* declared = parse_declare(world, program_name, optarg);
				if (!declared) {
					show_help(program_name);
					exitcode = 1;
					goto free_world;
				}
				break;
			}
			default:
				show_help(program_name);
				exitcode = 1;
				goto free_world;
		}
	}

	if (optind + 1 != argc) {
		fprintf(stderr, "%s: missing expression\n", program_name);
		show_help(program_name);
		exitcode = 1;
		goto free_world;
	}

	mat_parser_t* parser;
	mat_expr_t* e;
	parser = mat_parser_new(world, argv[optind]);
	e = mat_parser_parse(parser);
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
free_world:
	mat_world_free(world);
	return exitcode;
}

static void show_help(const char* program_name) {
	const char* help = ""
			"usage: %s [OPTIONS] EXPR\n"
			"OPTIONS:\n"
			"  -v, --variable NAME=EXPR   declare variable\n"
			"NAME must be single alphabet character.\n"
			"EXPR must be valid syntax expression.\n";
	fprintf(stderr, help, program_name);
}

static mat_expr_t* parse_declare(mat_world_t* world, const char* program_name, const char* expr_str) {
	if (isalpha(expr_str[0]) && expr_str[1] == '=') {
		mat_parser_t* parser = mat_parser_new(world, expr_str + 2);
		mat_expr_t* e = mat_parser_parse(parser);
		if (e) {
			mat_world_put_variable(world, (mat_variable_name_t) expr_str[0], e);
			mat_parser_free(parser);
			return e;
		} else {
			char buf[] = "<variable[x]>";
			buf[10] = expr_str[0];
			mat_parser_describe_error_position(parser, buf);
			mat_parser_free(parser);
		}
	} else {
		fprintf(stderr, "%s: wrong syntax passed to --variable\n", program_name);
	}
	return NULL;
}
