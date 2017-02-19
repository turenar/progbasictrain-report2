#include "config.inc.h"

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "expr.h"
#include "newton.h"
#include "parser.h"
#include "world.h"
#include "compat/gmp_wrapper.h"
#include "fns/fns.h"

static void show_help(const char* program_name);
/**
 * --declareに渡される引数をパースする
 * @param expr_str --declareに渡された引数
 * @return 正しい引数の形でなければNULL
 */
static mat_expr_t* parse_declare(mat_world_t*, const char* expr_str);
/**
 * (浮動小数点数な) 引数からmpq_tをsetする
 * @param target initされている必要がある
 * @param str 浮動小数点数な引数
 * @return 正しい浮動小数点である場合true
 */
static bool set_rational(const char* str, mpq_t target);
/**
 * (整数な) 引数からint型を取得する
 * @return 正しい
 */
static bool fully_strtoui(const char* arg_name, const char*, int*);

int is_local_extreme(mat_world_t* w, const mat_expr_t* df, const mpq_t x, const mpq_t epsilon);

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

	// 引数パースの準備
	mat_world_t* world = mat_world_new();
	mat_fn_put_stdfunc(world); // 標準関数の登録
	// ニュートン法パラメータの準備
	mat_newton_param_t param;
	mpq_inits(param.min, param.max, param.epsilon, param.initial, NULL);
	mpq_set_si(param.min, -100, 1);
	mpq_set_si(param.max, 100, 1);
	mpq_set_ui(param.epsilon, 1, 10000000);
	mpq_set_ui(param.initial, 1, 100);
	param.precision = 1024;
	param.patience = 32;

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
				if (!set_rational(optarg, param.min)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'M':
				if (!set_rational(optarg, param.min)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'e':
				if (!set_rational(optarg, param.epsilon)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'i':
				if (!set_rational(optarg, param.initial)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'p':
				if (!fully_strtoui("patience", optarg, &param.patience)) {
					exitcode = 1;
					goto free_world;
				}
				break;
			case 'P':
				if (!fully_strtoui("precision", optarg, &param.precision)) {
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

	mpq_t extreme_x; // 極値のx座標
	mpq_init(extreme_x);
	// 極値を取得するためにf'(x) = 0となるxを探すことに注意。
	if (mat_newton_optimize(world, df, &param, extreme_x)) {
		fprintf(stderr, "error: %s\n", mat_err_get(mat_world_get_error_info(world)));
		exitcode = 1;
		goto free_extreme_x;
	}

	mpq_t f_value, d2f_value;
	mpq_inits(f_value, d2f_value, NULL);
	// x=extreme_xにおけるf(x)とf''(x)の値を取得するために、xに値を代入する
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

	int is_extreme = is_local_extreme(world, df, extreme_x, param.epsilon);
	if (is_extreme < 0) {
		// error
		exitcode = 1;
		goto free_d2f_value;
	} else if (is_extreme) {
		printf("局所%s値: %f (x=%f)\n", (mpq_sgn(d2f_value) < 0 ? "最大" : "最小"), mpq_get_d(f_value), mpq_get_d(extreme_x));
	} else {
		printf("%f (x=%f) は局所%s値ではないかもしれません (変曲点の可能性あり)\n", mpq_get_d(f_value),
		       mpq_get_d(extreme_x), (mpq_sgn(d2f_value) < 0 ? "最大" : "最小"));
	}
	exitcode = 0;

	// C++のRAIIを使いたかった。怒涛の解放劇
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
			"\n"
			"shows extreme (in `x' axis) value analyzed by newton method\n"
			"\n"
			"OPTIONS:\n"
			"  -h, --help              show this help and exit\n"
			"  -D, --define NAME=EXPR  define EXPR as NAME. EXPR may or may not include 'x',\n"
			"                          but NAME must not be 'x'\n"
			"  -e, --epsilon FLOAT     set epsilon [default: 1e-8]\n"
			"  -i, --initial FLOAT     start with specific value. changing value may\n"
			"                          cause convergence or not [default: 0.01]\n"
			"  -m, --min FLOAT         if 'x' overs specific value in analysis, exit\n"
			"                          [default: -100]\n"
			"  -M, --max FLOAT         if 'x' overs specific value in analysis, exit\n"
			"                          [default: 100]\n"
			"  -p, --patience NUM      set maximum patience. larger value can sometimes \n"
			"                          converge newton method; but if not converged, \n"
			"                          just spend more time [default: 32]\n"
			"  -P, --precision NUM     set float precision in newton method. this option \n"
			"                          has no effect on result precision. if in newton method\n"
			"                          x movement is too small, larger precision just \n"
			"                          spend more time [default: 1024]\n"
			"NAME must be single alphabet character.\n"
			"EXPR must be valid syntax expression.\n";
	fprintf(stderr, help, program_name);
}

static mat_expr_t* parse_declare(mat_world_t* world, const char* expr_str) {
	if (expr_str[0] == 'x') {
		fprintf(stderr, "fatal: --define can't definition of `x'\n");
	} else if (isalpha(expr_str[0]) && expr_str[1] == '=') {
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

static bool set_rational(const char* str, mpq_t target) {
	mpf_t tmp;
	mpf_init(tmp);
	if (mpf_set_str(tmp, str, 10)) {
		fprintf(stderr, "fatal: illegal number: %s\n", str);
		mpq_clear(target);
		return false;
	} else {
		mpq_set_f(target, tmp);
		mpf_clear(tmp);
		return true;
	}
}

static bool fully_strtoui(const char* arg_name, const char* str, int* target) {
	char* endptr;
	*target = (int) strtol(str, &endptr, 10);
	bool success = *endptr == '\0' && *target >= 0;
	if (!success) {
		fprintf(stderr, "fatal: --%s requires positive number but `%s'\n", arg_name, str);
	}
	return success;
}

int is_local_extreme(mat_world_t* w, const mat_expr_t* df, const mpq_t x, const mpq_t epsilon) {
	int ret;

	mpq_t dx, a, b, df_a, df_b, multiplied;
	mpq_inits(dx, a, b, df_a, df_b, multiplied, NULL);
	mpq_mul_2exp(dx, epsilon, 4); // 16*epsilon
	mpq_add(a, x, dx);
	mpq_sub(b, x, dx);
	mat_world_put_variable(w, 'x', mat_expr_new_const(a));
	if (mat_op_calc_value(w, df, df_a)) {
		goto error;
	}
	mat_world_put_variable(w, 'x', mat_expr_new_const(b));
	if (mat_op_calc_value(w, df, df_b)) {
		goto error;
	}
	mpq_mul(multiplied, df_a, df_b);

	gmp_printf("%Qd %Qd\n", df_a, df_b);
	ret = mpq_sgn(multiplied) < 0;
	goto free_mpq;
error:
	fprintf(stderr, "error: %s\n", mat_err_get(mat_world_get_error_info(w)));
	ret = -1;
free_mpq:
	mpq_clears(dx, a, b, df_a, df_b, multiplied, NULL);
	return ret;
}
