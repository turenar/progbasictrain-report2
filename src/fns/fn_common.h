#pragma once

#include <stddef.h>
#include "../error.h"
#include "../fwddecl.h"
#include "compat/gmp_wrapper.h"
#include "compat/mpfr_wrapper.h"

void mat_fn_common_show_expression(mat_world_t* w, const mat_expr_t* expr);

typedef mat_error_t (* mat_bifunc_checker)(mat_world_t* w, mpq_srcptr, mpq_srcptr);
typedef mat_error_t (* mat_func_checker)(mat_world_t* w, mpq_srcptr);

typedef void (* mpq_bifunc)(mpq_ptr, mpq_srcptr, mpq_srcptr);
mat_error_t mat_fn_common_apply_bifunction(mat_world_t* w, const mat_expr_t*, mpq_t out,
                                           mat_bifunc_checker, mpq_bifunc);

typedef int (* mpfr_func)(mpfr_ptr, mpfr_srcptr, mpfr_rnd_t);
mat_error_t mat_fn_common_apply_mpfr_function(mat_world_t* w, const mat_expr_t*, mpq_t out,
                                              mat_func_checker, mpfr_func);

typedef int (* mpfr_bifunc)(mpfr_ptr, mpfr_srcptr, mpfr_srcptr, mpfr_rnd_t);
mat_error_t mat_fn_common_apply_mpfr_bifunction(mat_world_t* w, const mat_expr_t*, mpq_t out,
                                                mat_bifunc_checker, mpfr_bifunc);

mat_expr_t* mat_fn_common_plus(mat_expr_t*, mat_expr_t*);
mat_expr_t* mat_fn_common_subtract(mat_expr_t*, mat_expr_t*);
mat_expr_t* mat_fn_common_times(mat_expr_t*, mat_expr_t*);
mat_expr_t* mat_fn_common_divide(mat_expr_t*, mat_expr_t*);
