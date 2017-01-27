#include "config.inc.h"

#include "world.h"
#include "op_hash.h"
#include <stdlib.h>

struct mat_world {
	mat_op_hash_t* hash;
};
typedef struct mat_world mat_world_t;

mat_world_t* mat_world_new() {
	mat_world_t* ret = (mat_world_t*) malloc(sizeof(mat_world_t));
	ret->hash = mat_op_hash_new();
	return ret;
}

void mat_world_free(mat_world_t* w) {
	mat_op_hash_free(w->hash);
	free(w);
}

void mat_world_put_op(mat_world_t* w, const char* name, mat_op_def_t* def) {
	mat_op_hash_put(w->hash, name, def);
}

mat_op_def_t* mat_world_get_op(mat_world_t* w, const char* name) {
	return mat_op_hash_get(w->hash, name);
}
