#pragma once

#include "fwddecl.h"

mat_parser_t* mat_parser_new(mat_world_t*, const char*);
mat_expr_t* mat_parser_parse(mat_parser_t*);
void mat_parser_free(mat_parser_t*);
