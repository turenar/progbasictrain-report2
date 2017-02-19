#include "config.inc.h"

#include "mpfr_wrapper.h"

#ifdef USE_GMP
typedef int make_iso_compilers_happy; // suppress "ISO C forbids an empty translation unit"
#else

#include <math.h>

void mpfr_init(mpfr_ptr a) {
	*a = 0;
}

void mpfr_clear(mpfr_ptr a) {
	UNUSED_VAR(a);
}

void mpfr_set_q(mpq_ptr a, mpfr_srcptr b, mpfr_rnd_t c) {
	UNUSED_VAR(c);
	*a = *b;
}

void mpfr_get_f(mpf_ptr a, mpfr_srcptr b, mpfr_rnd_t c) {
	UNUSED_VAR(c);
	*a = *b;
}

int mpfr_sin(mpfr_ptr a, mpfr_srcptr b, mpfr_rnd_t c) {
	UNUSED_VAR(c);
	*a = sin(*b);
	return 0;
}

int mpfr_cos(mpfr_ptr a, mpfr_srcptr b, mpfr_rnd_t c) {
	UNUSED_VAR(c);
	*a = cos(*b);
	return 0;
}

int mpfr_log(mpfr_ptr a, mpfr_srcptr b, mpfr_rnd_t c) {
	UNUSED_VAR(c);
	*a = log(*b);
	return 0;
}

int mpfr_exp(mpfr_ptr a, mpfr_srcptr b, mpfr_rnd_t c) {
	UNUSED_VAR(c);
	*a = exp(*b);
	return 0;
}

int mpfr_pow(mpfr_ptr a, mpfr_srcptr b, mpfr_srcptr c, mpfr_rnd_t d) {
	UNUSED_VAR(d);
	*a = pow(*b, *c);
	return 0;
}

#endif
