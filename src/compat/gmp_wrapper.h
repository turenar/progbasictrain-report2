#pragma once

#include "config.inc.h"

#ifdef USE_GMP
#include <gmp.h>
#else

typedef unsigned long mp_bitcnt_t;
typedef double mpf_t[1];
typedef double mpq_t[1];
typedef double* mpf_ptr;
typedef double* mpq_ptr;
typedef const double* mpf_srcptr;
typedef const double* mpq_srcptr;

void mpf_init(mpf_ptr a);
void mpf_init2(mpf_ptr a, mp_bitcnt_t prec);
void mpq_init(mpq_ptr a);
void mpq_inits(mpq_ptr, ...);

int mpf_set_str(mpf_ptr a, const char* b, mp_bitcnt_t c);
void mpf_set_q(mpf_ptr a, mpf_srcptr b);
void mpq_set(mpq_ptr a, mpq_srcptr b);
void mpq_set_f(mpq_ptr a, const mpf_ptr b);
void mpq_set_d(mpq_ptr a, double b);
void mpq_set_si(mpq_ptr a, signed long b, signed long c);
void mpq_set_ui(mpq_ptr a, unsigned long b, unsigned long c);

double mpq_get_d(mpq_srcptr a);

int mpq_sgn(mpq_srcptr a);
int mpq_cmp(mpq_srcptr a, mpq_srcptr b);
int mpq_cmp_si(mpq_srcptr a, signed long b, signed long c);
int mpq_cmp_ui(mpq_srcptr a, unsigned long b, unsigned long c);

void mpq_abs(mpq_ptr a, mpq_srcptr b);
void mpq_add(mpq_ptr a, mpq_srcptr b, mpq_srcptr c);
void mpq_sub(mpq_ptr a, mpq_srcptr b, mpq_srcptr c);
void mpq_mul(mpq_ptr a, mpq_srcptr b, mpq_srcptr c);
void mpq_mul_2exp(mpq_ptr a, mpq_srcptr b, mp_bitcnt_t c);
void mpq_div(mpq_ptr a, mpq_srcptr b, mpq_srcptr c);

void mpf_clear(mpf_ptr);
void mpq_clear(mpq_ptr);
void mpq_clears(mpq_ptr, ...);

#endif
