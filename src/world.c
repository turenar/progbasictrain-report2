#include "config.inc.h"

#include "world.h"
#include <stdlib.h>
#include "op.h"
#include "op_hash.h"

struct mat_world {
	mat_op_hash_t* hash;
	mat_error_info_t err;
};
typedef struct mat_world mat_world_t;

mat_world_t* mat_world_new() {
	mat_world_t* ret = (mat_world_t*) malloc(sizeof(mat_world_t));
	ret->hash = mat_op_hash_new();
	mat_err_init(&ret->err);
	return ret;
}

void mat_world_free(mat_world_t* w) {
	mat_op_hash_free(w->hash);
	mat_err_clear(&w->err);
	free(w);
}

void mat_world_put_op(mat_world_t* w, const char* name, const mat_op_def_t* def) {
	mat_op_hash_put(w->hash, name, def);
}

const mat_op_def_t* mat_world_get_op(const mat_world_t* w, const char* name) {
	return mat_op_hash_get(w->hash, name);
}

mat_error_info_t* mat_world_get_error_info(mat_world_t* w) {
	return &w->err;
}
