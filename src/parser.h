#pragma once

#include "fwddecl.h"

mat_parser_t* mat_parser_new(mat_world_t*, const char*);
void mat_parser_free(mat_parser_t*);

mat_expr_t* mat_parser_parse(mat_parser_t*);
void mat_parser_describe_error_position(mat_parser_t*, const char* filename);
