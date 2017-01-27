#pragma once

#include "../fwddecl.h"
#include "../error.h"
#include <gmp.h>

void mat_fn_common_show_expression(const mat_expr_t* expr);

typedef void (*mpq_bifunc)(mpq_ptr, mpq_srcptr, mpq_srcptr);
mat_error_t mat_fn_common_apply_bifunction(const mat_expr_t*, mpq_t out, mpq_bifunc);
