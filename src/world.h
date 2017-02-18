#pragma once

#include "error.h"
#include "fwddecl.h"

mat_world_t* mat_world_new() __attribute_malloc__;
void mat_world_free(mat_world_t*);
void mat_world_put_op(mat_world_t*, const char* name, const mat_op_def_t*);
const mat_op_def_t* mat_world_get_op(const mat_world_t*, const char* name);
mat_error_info_t* mat_world_get_error_info(mat_world_t*);
