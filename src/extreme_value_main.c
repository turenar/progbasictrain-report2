#include "config.inc.h"

#include <ctype.h>
#include <getopt.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "expr.h"
#include "newton.h"
#include "parser.h"
#include "world.h"
#include "fns/fns.h"

static void show_help(const char* program_name);
static mat_expr_t* parse_declare(mat_world_t*, const char* expr_str);
static bool set_rational(mpq_t t, const char* str);
static bool fully_strtoi(const char*, int*);

int main(int argc, char** argv) {
	struct option longopts[] = {
			{"help",      no_argument,       NULL, 'h'},
			{"define",    required_argument, NULL, 'D'},
			{"epsilon",   required_argument, NULL, 'e'},
			{"initial",   required_argument, NULL, 'i'},
			{"min",       required_argument, NULL, 'm'},
			{"max",       required_argument, NULL, 'M'},
			{"patience",  required_argument, NULL, 'p'},
			{"precision", required_argument, NULL, 'P'},
			{NULL, 0,                        NULL, 0}
	};

	int exitcode;

	mat_world_t* world = mat_world_new();
	mat_fn_put_stdfunc(world);
	mat_newton_param_t param;
	mpq_inits(param.min, param.max, param.epsilon, param.initial, NULL);
	mpq_set_si(param.min, -100, 1);
	mpq_set_si(param.max, 100, 1);
	mpq_set_ui(param.epsilon, 1, 10000000);
	mpq_set_ui(param.initial, 1, 100);
	param.precision = 1024;
	param.patience = 30;

	const char* program_name = argv[0];
	int opt;
	int longindex;

	while ((opt = getopt_long(argc, argv, "hD:e:i:m:M:p:P:", longopts, &longindex)) != -1) {
		switch (opt) {
			case 'h':
				show_help(program_name);
				exitcode = 0;
				goto free_world;
			case 'D': {
				mat_expr_t* declared = parse_declare(world, optarg);
				if (!declared) {
					show_help(program_name);
					exitcode = 1;
					goto free_world;
				}
				break;
			}
			case 'm':
				if (!set_rational(param.min, optarg)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'M':
				if (!set_rational(param.min, optarg)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'e':
				if (!set_rational(param.epsilon, optarg)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'i':
				if (!set_rational(param.initial, optarg)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'p':
				if (!fully_strtoi(optarg, &param.patience)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'P':
				if (!fully_strtoi(optarg, &param.precision)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			default:
				show_help(program_name);
				exitcode = 1;
				goto free_world;
		}
	}

	if (optind + 1 != argc) {
		fprintf(stderr, "fatal: missing expression\n");
		show_help(program_name);
		exitcode = 1;
		goto free_world;
	} else if (mpq_cmp(param.min, param.max) >= 0) {
		fprintf(stderr, "fatal: min > max\n");
		exitcode = 1;
		goto free_world;
	}

	mat_parser_t* parser;
	mat_expr_t* f;
	parser = mat_parser_new(world, argv[optind]);
	f = mat_parser_parse(parser);
	if (!f) {
		mat_parser_describe_error_position(parser, "<input>");
		exitcode = 1;
		goto free_parser;
	}

	mat_expr_t* df;
	df = mat_op_make_differential(world, f);
	if (!df) {
		fprintf(stderr, "error: %s\n", mat_err_get(mat_world_get_error_info(world)));
		exitcode = 1;
		goto free_expr;
	}
	mat_expr_t* d2f;
	d2f = mat_op_make_differential(world, f);
	if (!d2f) {
		fprintf(stderr, "error: %s\n", mat_err_get(mat_world_get_error_info(world)));
		exitcode = 1;
		goto free_df;
	}

	mpq_t extreme_x;
	mpq_init(extreme_x);
	if (mat_newton_optimize(world, df, &param, extreme_x)) {
		fprintf(stderr, "error: %s\n", mat_err_get(mat_world_get_error_info(world)));
		exitcode = 1;
		goto free_extreme_x;
	}

	mpq_t f_value, d2f_value;
	mpq_inits(f_value, d2f_value, NULL);
	mat_world_put_variable(world, 'x', mat_expr_new_const(extreme_x));
	if (mat_op_calc_value(world, f, f_value)) {
		fprintf(stderr, "error: %s\n", mat_err_get(mat_world_get_error_info(world)));
		exitcode = 1;
		goto free_d2f_value;
	}
	if (mat_op_calc_value(world, d2f, d2f_value)) {
		fprintf(stderr, "error: %s\n", mat_err_get(mat_world_get_error_info(world)));
		exitcode = 1;
		goto free_d2f_value;
	}

	printf("局所%s値: %f (x=%f)\n", (mpq_sgn(d2f_value) < 0 ? "最大" : "最小"), mpq_get_d(f_value), mpq_get_d(extreme_x));
	exitcode = 0;

free_d2f_value:
	mpq_clears(f_value, d2f_value, NULL);
free_extreme_x:
	mpq_clear(extreme_x);
	mat_expr_free(d2f);
free_df:
	mat_expr_free(df);
free_expr:
	mat_expr_free(f);
free_parser:
	mat_parser_free(parser);
free_world:
	mpq_clears(param.min, param.max, param.epsilon, param.initial, NULL);
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

static mat_expr_t* parse_declare(mat_world_t* world, const char* expr_str) {
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
		fprintf(stderr, "fatal: wrong syntax passed to --variable\n");
	}
	return NULL;
}

static bool set_rational(mpq_t t, const char* str) {
	mpf_t tmp;
	mpf_init(tmp);
	if (mpf_set_str(tmp, str, 10)) {
		fprintf(stderr, "fatal: illegal number: %s\n", str);
		mpq_clear(t);
		return false;
	} else {
		mpq_set_f(t, tmp);
		mpf_clear(tmp);
		return true;
	}
}

static bool fully_strtoi(const char* str, int* target) {
	char* endptr;
	*target = (int) strtol(str, &endptr, 10);
	return *endptr == '\0';
}
