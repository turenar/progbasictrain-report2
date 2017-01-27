#pragma once

#include "fwddecl.h"

mat_world_t* mat_world_new() __attribute_malloc__;
void mat_world_free(mat_world_t*);
void mat_world_put_stdfunc(mat_world_t*);
void mat_world_put_op(mat_world_t*, const char* name, mat_op_def_t*);
mat_op_def_t* mat_world_get_op(mat_world_t*, const char* name);
