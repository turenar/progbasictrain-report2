#pragma once

#include <gmp.h>
#include "fwddecl.h"
#include "error.h"

mat_error_t mat_newton_optimize(mat_world_t*, const mat_expr_t*, const mpq_t initial, const mpq_t epsilon, mpq_t out);
