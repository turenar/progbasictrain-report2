#pragma once

#include "fwddecl.h"

mat_op_hash_t* mat_op_hash_new();
void mat_op_hash_put(mat_op_hash_t*, const char* name, mat_op_def_t*);
mat_op_def_t* mat_op_hash_get(mat_op_hash_t*, const char*);
void mat_op_hash_free(mat_op_hash_t*);
