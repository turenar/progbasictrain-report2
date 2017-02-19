#pragma once

#include "config.inc.h"

#ifdef USE_GMP
#include <mpfr.h>
#else

#include "compat/gmp_wrapper.h"

typedef unsigned char mpfr_rnd_t;
typedef double mpfr_t[1];
typedef double* mpfr_ptr;
typedef const double* mpfr_srcptr;

#define MPFR_RNDN 0

void mpfr_init(mpfr_ptr);
void mpfr_clear(mpfr_ptr);

void mpfr_set_q(mpq_ptr, mpfr_srcptr, mpfr_rnd_t);
void mpfr_get_f(mpf_ptr, mpfr_srcptr, mpfr_rnd_t);

int mpfr_sin(mpfr_ptr, mpfr_srcptr, mpfr_rnd_t);
int mpfr_cos(mpfr_ptr, mpfr_srcptr, mpfr_rnd_t);
int mpfr_exp(mpfr_ptr, mpfr_srcptr, mpfr_rnd_t);
int mpfr_log(mpfr_ptr, mpfr_srcptr, mpfr_rnd_t);
int mpfr_pow(mpfr_ptr, mpfr_srcptr, mpfr_srcptr, mpfr_rnd_t);
#endif
