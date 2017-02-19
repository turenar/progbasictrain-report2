#include "config.inc.h"

#include "world.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "op.h"
#include "op_hash.h"

struct mat_world {
	mat_op_hash_t* hash;
	mat_error_info_t err;
	mat_expr_t* var_values[UCHAR_MAX];
};
typedef struct mat_world mat_world_t;

mat_world_t* mat_world_new() {
	mat_world_t* ret = (mat_world_t*) malloc(sizeof(mat_world_t));
	ret->hash = mat_op_hash_new();
	mat_err_init(&ret->err);
	memset(ret->var_values, 0, sizeof(ret->var_values));
	return ret;
}

void mat_world_free(mat_world_t* w) {
	if (w) {
		mat_op_hash_free(w->hash);
		mat_err_clear(&w->err);
		for (int i = 0; i < UCHAR_MAX; ++i) {
			mat_expr_free(w->var_values[i]);
		}
		free(w);
	}
}

void mat_world_put_op(mat_world_t* w, const char* name, const mat_op_def_t* def) {
	mat_op_hash_put(w->hash, name, def);
}

const mat_op_def_t* mat_world_get_op(const mat_world_t* w, const char* name) {
	return mat_op_hash_get(w->hash, name);
}

void mat_world_put_variable(mat_world_t* w, mat_variable_name_t variable_name, mat_expr_t* e) {
	if (w->var_values[variable_name]) {
		mat_expr_free(w->var_values[variable_name]);
	}
	w->var_values[variable_name] = e;
}

mat_expr_t* mat_world_get_variable(mat_world_t* w, mat_variable_name_t variable_name) {
	return w->var_values[variable_name];
}

mat_error_info_t* mat_world_get_error_info(mat_world_t* w) {
	return &w->err;
}
