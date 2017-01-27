#pragma once

#include <gmp.h>
#include <mpfr.h>
#include "../error.h"
#include "../fwddecl.h"

void mat_fn_common_show_expression(const mat_expr_t* expr);

typedef void (*mpq_bifunc)(mpq_ptr, mpq_srcptr, mpq_srcptr);
mat_error_t mat_fn_common_apply_bifunction(const mat_expr_t*, mpq_t out, mpq_bifunc);

typedef int (*mpfr_func)(mpfr_ptr, mpfr_srcptr,mpfr_rnd_t);
mat_error_t mat_fn_common_apply_mpfr_function(const mat_expr_t*, mpq_t out, mpfr_func);
